// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ThreadManagerSubsystem.generated.h"

class UOptComp;
class FWorkThread;

/**
 * �ڵ����� ���� ����
 */
UCLASS()
class MASSCROWD_API UThreadManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override final;
	virtual void Deinitialize() override final;

	void StartThread();
	void EndThread();

public: 
	// ���� �������̽���
	void RegisterComponent(UOptComp* Component) const;
	void UnregisterComponent(const UOptComp* Component) const;

private:
	TObjectPtr<UWorld> World = nullptr;
	FWorkThread* Thread;
	FCriticalSection Mutex;
};
