// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/OptData.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"

class UOptComp;
class APlayerCameraManager;
/**
 * 
 */
class MASSCROWD_API FWorkThread : public FRunnable, public FGCObject
{
public:
	FWorkThread(UWorld* InWorld) : Thread(nullptr), bIsRunning(false), World(InWorld) {
	}

	virtual ~FWorkThread() override {
		Stop();
	}
	void Work();
	void RegisterComp(UOptComp* Comp);
	void UnregisterComp(const UOptComp* Comp);

public: 
	// FRunnable
	virtual uint32 Run() override final;
	virtual void Stop() override final;

public:
	// 참조 문제 방지를 위한 GC
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override {
		Collector.AddReferencedObject(World);
		for (auto& Comp : Components)
			Collector.AddReferencedObject(Comp);
	}
	virtual FString GetReferencerName() const override { return TEXT("FOptThread"); }

private:
	void RemoveComp(const int32 Handle);
	static EOptState CalculArea(const AActor* Player, const AActor* Bot, const float InFOV = 90.f);
	static bool IsInViewPort(const APlayerCameraManager* Camera, const AActor* Bot);

private:
	TArray<TObjectPtr<UOptComp>> Components;
	FCriticalSection Mutex;
	TUniquePtr<FRunnableThread> Thread;
	FThreadSafeBool bIsRunning;
	TObjectPtr<UWorld> World;
	
};
