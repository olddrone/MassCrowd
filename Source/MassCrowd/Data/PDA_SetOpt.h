// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDA_SetOpt.generated.h"

/**
 * 
 */
UCLASS()
class MASSCROWD_API UPDA_SetOpt : public UPrimaryDataAsset
{
	GENERATED_BODY()
public: // Skeletal Mesh
	UPROPERTY(EditAnywhere)
	bool bPauseAnim = false;
	
	UPROPERTY(EditAnywhere)
	bool bCastShadow = true;
	
	UPROPERTY(EditAnywhere)
	bool bSetVisibility = true;

	UPROPERTY(EditAnywhere)
	bool bBoneMotionBlur = true;
	
	UPROPERTY(EditAnywhere)
	int32 ForcedLodModel = 0;

	UPROPERTY(EditAnywhere)
	bool bAllowRigidBodyAnimNode = true;

	UPROPERTY(EditAnywhere)
	bool bCollisionEnable = false;
	
	UPROPERTY(EditAnywhere)
	float TickInterval = 0.02f; 

	UPROPERTY(EditAnywhere)
	bool bOverlapEvent = true;

public: // Movement Component
	UPROPERTY(EditAnywhere)
	bool bMoveEnablePhysicsInteraction = true;
	
	UPROPERTY(EditAnywhere)
	int32 MoveMaxSimIter = 8;
	
	UPROPERTY(EditAnywhere)
	bool bCheckFloor = true;	// 틱 마다 바닥으로 라인 트레이싱

	UPROPERTY(EditAnywhere)
	bool bMoveUseAccel = true;	// 가속도 계산

	UPROPERTY(EditAnywhere)
	bool bSweepWhileWalking = true;	// 가속도 계산

};
