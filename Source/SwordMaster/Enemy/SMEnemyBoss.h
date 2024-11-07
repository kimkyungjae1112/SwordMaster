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

	virtual float GetDetectRadius() override;
	virtual float GetPatrolRadius() override;
	virtual float GetAttackRange() override;
	virtual void SetAttackFinished(const FOnAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;
	virtual void AttackEndTiming() override;
	virtual void DefaultAttackHitCheck() override;

	FOnAttackFinished OnAttackFinished;

public:
	/* 패링 공격 피격 */
	void BeginParryingAttackHit();

/* 메쉬 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> WeaponMeshComp;

	/* 패링 공격 피격 */
	void EndParryingAttackHit(class UAnimMontage* Target, bool IsProperlyEnded);
	void ParryingAttackHitMotionWarpSet();


/* 기본 공격 섹션 */
private:
	void BeginDefaultAttack();
	void EndDefaultAttack(class UAnimMontage* Target, bool IsProperlyEnded);

	UPROPERTY(EditAnywhere, Category = "Combo")
	TObjectPtr<class UEnemyDefaultAttackData> DefaultAttackData;

	bool bCanAttack = true;
	int32 CurrentCombo = 1;

	void DefaultAttackMotionWarpSet();
	bool DetectInRange(TArray<FOverlapResult>& InOverlapResults);

	//플래그는 true
	//기본 공격 실행 시 플래그 false
	//공격 범위 췤
	//그 다음 노티파이 다음 섹션으로 이동 (공격 가능하면)
	//아니라면 콤보 0으로 초기화

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
