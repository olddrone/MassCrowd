// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bot.generated.h"

class UOptComp;

UCLASS()
class MASSCROWD_API ABot : public ACharacter
{
	GENERATED_BODY()

public:
	ABot();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UOptComp> OptComp;

};
