// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FOnAttackFinished);

/**
 * 
 */
class SWORDMASTER_API IAIInterface
{
	GENERATED_BODY()

public:
	virtual float GetDetectRadius() = 0;
	virtual float GetPatrolRadius() = 0;
	virtual float GetAttackRange() = 0;

	virtual void SetAttackFinished(const FOnAttackFinished& InOnAttackFinished) = 0;
	virtual void AttackByAI() = 0;
	virtual void AttackEndTiming() = 0;

	virtual void DefaultAttackHitCheck() = 0;
};
