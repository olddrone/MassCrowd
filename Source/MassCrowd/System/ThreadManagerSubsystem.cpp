// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreadManagerSubsystem.h"
#include "Thread/WorkThread.h"

void UThreadManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Error, TEXT("InstSubsystem Init"));

	if (!Thread)
		Thread = new FWorkThread(GetWorld());
}

void UThreadManagerSubsystem::Deinitialize()
{
	if (Thread) {
		Thread->Stop();
		delete Thread;
		Thread = nullptr;
	}
	UE_LOG(LogTemp, Error, TEXT("InstSubsystem Deinit"));
	Super::Deinitialize();
}

void UThreadManagerSubsystem::StartThread()
{
	if (Thread)
		Thread->Work();
}

void UThreadManagerSubsystem::EndThread()
{
	if (Thread)
		Thread->Stop();
}

void UThreadManagerSubsystem::RegisterComponent(UOptComp* Component) const
{
	if (Thread)
		Thread->RegisterComp(Component);
}

void UThreadManagerSubsystem::UnregisterComponent(const UOptComp* Component) const
{
	if (Thread)
		Thread->UnregisterComp(Component);
}
