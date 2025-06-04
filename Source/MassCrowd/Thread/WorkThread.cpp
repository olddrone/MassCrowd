// Fill out your copyright notice in the Description page of Project Settings.

#include "WorkThread.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Pawn.h"
#include "Component/OptComp.h"

static FString ToString(EOptState State)
{
	switch (State)
	{
	case EOptState::EOS_Default: return TEXT("Default");
	case EOptState::EOS_First:   return TEXT("First");
	case EOptState::EOS_Second:  return TEXT("Second");
	default:                     return TEXT("Unknown");
	}
}

uint32 FWorkThread::Run()
{
	while (bIsRunning)
	{
		APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(World, 0);
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		APawn* Player = PC ? PC->GetPawn() : nullptr;

		Mutex.Lock();
		{
			for (auto& Component : Components) {
				EOptState BeforeState = Component->GetOptState();
				EOptState State;

				if (IsInViewPort(Camera, Component->GetOwner()))
					State = CalculArea(Player, Component->GetOwner());
				else
					State = EOptState::EOS_Third;

				if (BeforeState != State)
					Component->SetOptState(State);
				
			}
		}
		Mutex.Unlock();
		FPlatformProcess::Sleep(0.03f);
	}

	return 0;
}

void FWorkThread::Stop()
{
	bIsRunning = false;
	if (Thread.IsValid())
	{
		Thread->WaitForCompletion();
		Thread.Reset();
	}
}

void FWorkThread::Work()
{
	if (!Thread.IsValid() && IsValid(World)) {
		bIsRunning = true;
		Thread.Reset(FRunnableThread::Create(this, *FString::Printf(TEXT("OptThread"))));
	}
}

void FWorkThread::RegisterComp(UOptComp* Comp)
{
	Mutex.Lock();
	{
		if (Comp->GetHandle() == INDEX_NONE) {
			Comp->SetHandle(Components.Num());
			Components.Add(Comp);
		}
	}
	Mutex.Unlock();
}

void FWorkThread::UnregisterComp(const UOptComp* Comp)
{
	Mutex.Lock();
	{
		const uint32 Handle = Comp->GetHandle();
		if (Handle != INDEX_NONE)
			RemoveComp(Handle);
	}
	Mutex.Unlock();
}

void FWorkThread::RemoveComp(const int32 Handle)
{
	if (Components.IsValidIndex(Handle)) 
	{
		if (IsValid(Components[Handle]))
			Components[Handle]->SetHandle(INDEX_NONE);

		Components.RemoveAtSwap(Handle, 1, false);

		const int32 Cnt = Components.Num();
		if (Cnt > 0 && Handle != Cnt) {
			if (IsValid(Components[Handle]))
				Components[Handle]->SetHandle(Handle);
		}

	}
}

EOptState FWorkThread::CalculArea(const AActor* Player, const AActor* Bot, const float InFOV)
{
	if (!IsValid(Player))
		return EOptState::EOS_Default;

	EOptState State;
	float Dist = FVector::Dist(Player->GetActorLocation(), Bot->GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("Dist : %f"), Dist);

	if (Dist > OptArea::Second)
		State = EOptState::EOS_Third;
	else if (Dist > OptArea::First)
		State = EOptState::EOS_Second;
	else if (Dist > OptArea::Default)
		State = EOptState::EOS_First;
	else
		State = EOptState::EOS_Default;


	return State;
}

bool FWorkThread::IsInViewPort(const APlayerCameraManager* Camera, const AActor* Bot)
{
	if(!IsValid(Camera) || !IsValid(Bot))
		return false;

	// 카메라 기준 대상 방향 벡터 계산
	const FVector ToTarget = (Bot->GetActorLocation() - Camera->GetCameraLocation()).GetSafeNormal();
	const FVector CameraForward = Camera->GetActorForwardVector();

	// 카메라의 FOV → 라디안 → 코사인값
	// FOV = Camera->GetFOVAngle() 하고 싶은데 게임스레드 접근 시 문제가 나는듯
	const float FOV = 90.f;
	const float FOVRadian = FMath::DegreesToRadians(FOV);
	const float CosValue = FMath::Cos(FOVRadian/2);

	// Dot Product를 통한 시야각 판별
	const float Dot = FVector::DotProduct(CameraForward, ToTarget);
	return Dot > CosValue;
}
