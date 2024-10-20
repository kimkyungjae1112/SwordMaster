// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Controller/AIControllerBase.h"
#include "AIControllerBoss.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMASTER_API AAIControllerBoss : public AAIControllerBase
{
	GENERATED_BODY()

public:
	AAIControllerBoss();
	
	virtual void Tick(float DeltaTime) override;

private:
	float Interval = 0.f;
};
