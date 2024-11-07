// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SMCharacterBase.h"
#include "Interface/AIInterface.h"
#include "SMEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMASTER_API ASMEnemyBase : public ASMCharacterBase, public IAIInterface
{
	GENERATED_BODY()
	
public:
	virtual float GetDetectRadius() override;
	virtual float GetPatrolRadius() override;
	virtual float GetAttackRange() override;
	virtual void SetAttackFinished(const FOnAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;
	virtual void AttackEndTiming() override;
	virtual void DefaultAttackHitCheck() override;


};
