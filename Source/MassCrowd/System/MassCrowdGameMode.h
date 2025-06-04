// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MassCrowdGameMode.generated.h"

UCLASS(minimalapi)
class AMassCrowdGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMassCrowdGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
};



