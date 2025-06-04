// Copyright Epic Games, Inc. All Rights Reserved.

#include "MassCrowdGameMode.h"
#include "Character/MassCrowdCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ThreadManagerSubsystem.h"


AMassCrowdGameMode::AMassCrowdGameMode()
{
	
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
		DefaultPawnClass = PlayerPawnBPClass.Class;

	
}

void AMassCrowdGameMode::BeginPlay()
{
	Super::BeginPlay();
	if(UThreadManagerSubsystem* Manager = GetWorld()->GetSubsystem<UThreadManagerSubsystem>())
		Manager->StartThread();
}

void AMassCrowdGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UThreadManagerSubsystem* Manager = GetWorld()->GetSubsystem<UThreadManagerSubsystem>())
		Manager->EndThread();

	Super::EndPlay(EndPlayReason);
}
