// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SMCharacterBase.h"
#include "SMEnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMASTER_API ASMEnemyBoss : public ASMCharacterBase
{
	GENERATED_BODY()
	
public:
	ASMEnemyBoss();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);



/* 히트 액션 섹션 */
private:
	void BeginProgressAttackHit();
	void EndProgressAttackHit(class UAnimMontage* Target, bool IsProperlyEnded);

	int32 CurrentHit = 0;

	UPROPERTY(EditAnywhere, Category = "HitData")
	TObjectPtr<class UEnemyHitData> ProgressAttackHitData;

/* 모션 워핑 섹션 */
private:

};
