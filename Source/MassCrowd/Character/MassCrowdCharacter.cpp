// Copyright Epic Games, Inc. All Rights Reserved.

#include "MassCrowdCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AMassCrowdCharacter::AMassCrowdCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; 
}

void AMassCrowdCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMassCrowdCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector Location = GetActorLocation();
	DrawDebugSphere(GetWorld(),Location, 800, 20, FColor::Red, false, -1);
	DrawDebugSphere(GetWorld(),Location, 1600, 20, FColor::Green, false, -1);
	DrawDebugSphere(GetWorld(),Location, 2400, 20, FColor::Blue, false, -1);
	
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		APlayerCameraManager* Camera = PC->PlayerCameraManager;

		const FVector CamLocation = Camera->GetCameraLocation();
		const FRotator CamRotation = Camera->GetCameraRotation();
		const FVector CamForward = CamRotation.Vector();
		const float FOV = Camera->GetFOVAngle();
		const float ViewDistance = 1200.0f; // 원하는 시야 거리

		// 뷰포트의 반 FOV 각도로 좌우 방향 벡터 계산
		const float HalfFOV = FMath::DegreesToRadians(FOV * 0.5f);
		const float AspectRatio = 16.0f / 9.0f;
		const float HalfHeight = FMath::Tan(HalfFOV) * ViewDistance;
		const float HalfWidth = HalfHeight * AspectRatio;

		// 카메라 로컬 축 기준
		const FVector Right = FRotationMatrix(CamRotation).GetUnitAxis(EAxis::Y);
		const FVector Up = FRotationMatrix(CamRotation).GetUnitAxis(EAxis::Z);

		const FVector Center = CamLocation + CamForward * ViewDistance;

		const FVector TopLeft = Center + (Up * HalfHeight) - (Right * HalfWidth);
		const FVector TopRight = Center + (Up * HalfHeight) + (Right * HalfWidth);
		const FVector BottomLeft = Center - (Up * HalfHeight) - (Right * HalfWidth);
		const FVector BottomRight = Center - (Up * HalfHeight) + (Right * HalfWidth);

		//카메라에서 4개 모서리까지 라인
		DrawDebugLine(GetWorld(), CamLocation, TopLeft, FColor::Yellow, false, -1, 0, 5);
		DrawDebugLine(GetWorld(), CamLocation, TopRight, FColor::Yellow, false, -1, 0, 5);
		DrawDebugLine(GetWorld(), CamLocation, BottomLeft, FColor::Yellow, false, -1, 0, 5);
		DrawDebugLine(GetWorld(), CamLocation, BottomRight, FColor::Yellow, false, -1, 0, 5);

		// 네 개의 모서리를 선으로 연결 (프러스텀)
		DrawDebugLine(GetWorld(), TopLeft, TopRight, FColor::Yellow, false, -1, 0, 5);
		DrawDebugLine(GetWorld(), TopRight, BottomRight, FColor::Yellow, false, -1, 0, 5);
		DrawDebugLine(GetWorld(), BottomRight, BottomLeft, FColor::Yellow, false, -1, 0, 5);
		DrawDebugLine(GetWorld(), BottomLeft, TopLeft, FColor::Yellow, false, -1, 0, 5);
	}

}

void AMassCrowdCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMassCrowdCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMassCrowdCharacter::Look);
	}
}

void AMassCrowdCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMassCrowdCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}