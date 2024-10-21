// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDefaultAttackData.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMASTER_API UEnemyDefaultAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Name")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = "Count")
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = "Rate")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "Name")
	TArray<float> EffectiveFrameCount;

};
