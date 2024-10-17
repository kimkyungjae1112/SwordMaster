// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyHitData.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMASTER_API UEnemyHitData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Name")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = "Count")
	uint8 MaxHitCount;

	UPROPERTY(EditAnywhere, Category = "Rate")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "Name")
	TArray<float> EffectiveFrameCount;
};
