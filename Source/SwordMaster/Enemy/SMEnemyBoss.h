// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SMCharacterBase.h"
#include "Interface/AIInterface.h"
#include "SMEnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMASTER_API ASMEnemyBoss : public ASMCharacterBase, public IAIInterface
{
	GENERATED_BODY()
	
public:
	ASMEnemyBoss();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	
public:
	virtual float GetDetectRadius() override;
	virtual float GetPatrolRadius() override;
	virtual float GetAttackRange() override;
	virtual void SetAttackFinished(const FOnAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FOnAttackFinished OnAttackFinished;
/* 메쉬 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> WeaponMeshComp;

/* 히트 액션 섹션 */
private:
	void BeginProgressAttackHit();
	void EndProgressAttackHit(class UAnimMontage* Target, bool IsProperlyEnded);

	int32 CurrentHit = 0;
	bool HasNextHit1 = false;

	UPROPERTY(EditAnywhere, Category = "HitData")
	TObjectPtr<class UEnemyHitData> ProgressAttackHitData;

/* 공격 섹션 */
private:
	void BeginDefaultAttack();
	void EndDefaultAttack(class UAnimMontage* Target, bool IsProperlyEnded);


/* 몽타주 섹션 */
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

};
