// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/OptData.h"
#include "Data/PDA_SetOpt.h"
#include "OptComp.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MASSCROWD_API UOptComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UOptComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetHandle(const uint32& InHandle) { Handle = InHandle; }
	FORCEINLINE uint32 GetHandle() const { return Handle; }

	FORCEINLINE void SetOptState(const EOptState& InState) {
		FScopeLock Lock(&Section);
		OptState = InState;
	}
	FORCEINLINE EOptState GetOptState() const {
		FScopeLock Lock(&Section);
		return OptState;
	}

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void DrawDebug();
	void SetupOptimization();

private:
	mutable FCriticalSection Section;
	uint32 Handle = INDEX_NONE;
	EOptState OptState = EOptState::EOS_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	TArray<TObjectPtr<UPDA_SetOpt>> OptData;

	UPROPERTY()
	TObjectPtr<class ACharacter> Player;
	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> MeshComp;
	
	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> MoveComp;

};
