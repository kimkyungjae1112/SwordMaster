// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/SMEnemyBase.h"
#include "SMEnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMASTER_API ASMEnemyBoss : public ASMEnemyBase
{
	GENERATED_BODY()
	
public:
	ASMEnemyBoss();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual float GetAttackRange() override;
	virtual void SetAttackFinished(const FOnAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;
	virtual void AttackEndTiming() override;
	virtual void DefaultAttackHitCheck() override;

	FOnAttackFinished OnAttackFinished;

public:
	/* 패링 공격 피격 */
	void BeginParryingAttackHit();

	/* 패링 공격 피격 */
	void EndParryingAttackHit(class UAnimMontage* Target, bool IsProperlyEnded);
	void ParryingAttackHitMotionWarpSet();


/* 기본 공격 섹션 */
private:
	void BeginDefaultAttack();
	void EndDefaultAttack(class UAnimMontage* Target, bool IsProperlyEnded);
	void DefaultAttackMotionWarpSet();

	UPROPERTY(EditAnywhere, Category = "Combo")
	TObjectPtr<class UEnemyDefaultAttackData> DefaultAttackData;

	bool bCanAttack = true;
	int32 CurrentCombo = 1;

	bool MeleeAttackDetectInRange(TArray<FOverlapResult>& InOverlapResults);

/* 플레이어 포인터 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Player")
	TObjectPtr<class ACharacter> Player;

/* 메쉬 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> WeaponMeshComp;

/* 몽타주 섹션 */
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> DefaultAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> ParryingAttackHitMontage;

/* 컨트롤러 */
private:
	UPROPERTY()
	TObjectPtr<class AAIControllerBoss> MyController;

/* 모션 워핑 */
private:
	UPROPERTY(VisibleAnywhere, Category = "MotionWarp")
	TObjectPtr<class UMotionWarpingComponent> MotionWarpComp;

	UPROPERTY(VisibleAnywhere, Category = "MotionWarp")
	TObjectPtr<class UMotionWarpingComponent> ParryingHitWarpComp;
};
