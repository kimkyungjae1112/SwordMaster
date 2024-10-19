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
	UPROPERTY(EditAnywhere, Category = "Montage")
	TArray<class UAnimMontage*> ProgressAttackHitMontages;
};
