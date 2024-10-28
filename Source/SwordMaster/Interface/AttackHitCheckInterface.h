// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackHitCheckInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackHitCheckInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SWORDMASTER_API IAttackHitCheckInterface
{
	GENERATED_BODY()

public:
	virtual void ProgressAttackHitCheck() = 0;
	virtual void Q_SkillHitCheck() = 0;
};
