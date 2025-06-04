// Fill out your copyright notice in the Description page of Project Settings.

#include "Bot.h"
#include "Component/OptComp.h"
#include "Components/CapsuleComponent.h"

ABot::ABot()
{
	PrimaryActorTick.bCanEverTick = false;

	OptComp = CreateDefaultSubobject<UOptComp>(TEXT("OptComp"));
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABot::BeginPlay()
{
	Super::BeginPlay();
	
}
