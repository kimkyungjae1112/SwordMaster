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
	void BeginHit1();
	void EndHit1(class UAnimMontage* Target, bool IsProperlyEnded);
	void SetTimerHit1();
	void Hit1TimerCheck();

	int32 Hit1Count = 0;
	bool HasNextHitAction = false;
	FTimerHandle Hit1TimerHandle;

	UPROPERTY(EditAnywhere, Category = "HitData")
	TObjectPtr<class UEnemyHitData> HitData;

/* 몽타주 섹션 */
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> Hit1Montage;
};
