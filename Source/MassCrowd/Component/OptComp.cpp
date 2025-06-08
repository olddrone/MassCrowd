// Fill out your copyright notice in the Description page of Project Settings.

#include "OptComp.h"
#include "System/ThreadManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

UOptComp::UOptComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPDA_SetOpt> RefDefault(TEXT("/Game/Data/PDA_OptDefault"));
	if(RefDefault.Succeeded())
		OptData.Add(RefDefault.Object);

	static ConstructorHelpers::FObjectFinder<UPDA_SetOpt> RefFirst(TEXT("/Game/Data/PDA_OptFirst"));
	if (RefFirst.Succeeded())
		OptData.Add(RefFirst.Object);

	static ConstructorHelpers::FObjectFinder<UPDA_SetOpt> RefSecond(TEXT("/Game/Data/PDA_OptSecond"));
	if (RefSecond.Succeeded())
		OptData.Add(RefSecond.Object);

	static ConstructorHelpers::FObjectFinder<UPDA_SetOpt> RefThird(TEXT("/Game/Data/PDA_OptThird"));
	if (RefThird.Succeeded())
		OptData.Add(RefThird.Object);
}

void UOptComp::BeginPlay()
{
	Super::BeginPlay();

	if (UThreadManagerSubsystem* Manager = GetWorld()->GetSubsystem<UThreadManagerSubsystem>())
		Manager->RegisterComponent(this);

	Player = Cast<ACharacter>(GetOwner());
	MeshComp = Player->GetMesh();
	MoveComp = Player->GetCharacterMovement();
}

void UOptComp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UThreadManagerSubsystem* Manager = GetWorld()->GetSubsystem<UThreadManagerSubsystem>())
		Manager->UnregisterComponent(this);

	Super::EndPlay(EndPlayReason);
}

void UOptComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//DrawDebug();
	SetupOptimization();
}

void UOptComp::DrawDebug()
{
	FLinearColor Color = FLinearColor::Red;
	switch (OptState)
	{
	case EOptState::EOS_Third:
		Color = FColor::Yellow;
		break;
	case EOptState::EOS_Second:
		Color = FColor::Blue;
		break;
	case EOptState::EOS_First:
		Color = FColor::Green;
		break;
	case EOptState::EOS_Default:
		Color = FLinearColor::Red;
		break;
	}
	FVector Origin, Extent;
	GetOwner()->GetActorBounds(true, Origin, Extent);
	UKismetSystemLibrary::DrawDebugBox(GetWorld(), Origin, Extent, Color,
		FRotator::ZeroRotator, PrimaryComponentTick.TickInterval);
}

void UOptComp::SetupOptimization()
{
	UPDA_SetOpt* Data = OptData[static_cast<int>(OptState)];
	SkeletalMeshOptimization(Data);
	MovementCompOptimization(Data);
}

void UOptComp::SkeletalMeshOptimization(const UPDA_SetOpt* Data)
{
	if (!IsValid(MeshComp) || !IsValid(Data))
		return;

	MeshComp->bPauseAnims = Data->bPauseAnim;
	MeshComp->SetCastShadow(Data->bCastShadow);
	MeshComp->SetVisibility(Data->bSetVisibility);
	MeshComp->bPerBoneMotionBlur = Data->bBoneMotionBlur;
	MeshComp->ForcedLodModel = Data->ForcedLodModel;
	MeshComp->SetAllowRigidBodyAnimNode(Data->bAllowRigidBodyAnimNode);
	ECollisionEnabled::Type CollisionType = (Data->bCollisionEnable) ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;
	MeshComp->SetCollisionEnabled(CollisionType);
	MeshComp->SetComponentTickInterval(Data->TickInterval);
	MeshComp->SetGenerateOverlapEvents(Data->bOverlapEvent);
}

void UOptComp::MovementCompOptimization(const UPDA_SetOpt* Data)
{
	if (!IsValid(MoveComp) || !IsValid(Data))
		return;

	MoveComp->bEnablePhysicsInteraction = Data->bMoveEnablePhysicsInteraction;
	MoveComp->SetComponentTickInterval(Data->TickInterval);
	MoveComp->MaxSimulationIterations = Data->MoveMaxSimIter;
	MoveComp->bAlwaysCheckFloor = Data->bCheckFloor;
	MoveComp->bRequestedMoveUseAcceleration = Data->bMoveUseAccel;
}
