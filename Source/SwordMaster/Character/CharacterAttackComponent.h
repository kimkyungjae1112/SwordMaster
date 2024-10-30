// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttackHitCheckInterface.h"
#include "CharacterAttackComponent.generated.h"

DECLARE_DELEGATE(FOnParryingSign)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDMASTER_API UCharacterAttackComponent : public UActorComponent, public IAttackHitCheckInterface
{
	GENERATED_BODY()

public:	
	UCharacterAttackComponent();

protected:
	virtual void BeginPlay() override;

public:
	FOnParryingSign OnParryingSign;
	
	FORCEINLINE bool GetGuardFlag() { return bGuard; }
	FORCEINLINE bool GetParryingFlag() { return bParrying; }

/* 공격 */
public:
	void ProgressAttack();

private:
	/* 기본 공격 */
	void BeginProgressAttack();
	void EndProgressAttack(class UAnimMontage* Target, bool IsProperlyEnded);
	void SetProgressAttackTimer();
	void ProgressAttackCheck();

	UPROPERTY(VisibleAnywhere, Category = "AttackData")
	TObjectPtr<class UCharacterProgressAttackData> ProgressAttackData;

	/* PA = Progress Attack */
	int32 CurrentPA = 0;
	bool HasNextPACommand = false;
	FTimerHandle PATimerHandle;


	/* Motion Warp */
	void ProgressAttackTargetSet();
	void ProgressAttackRemoveTarget();

	/* Attack Range */
	bool ProgressAttackSphereCheck(TArray<FOverlapResult>& OverlapResults);
	bool ProgressAttackInDegree(AActor* Actor, float Degree);
	
	/* Hit Check */
	virtual void ProgressAttackHitCheck() override;


/* 방어 */
public:
	void BeginParrying();
	void BeginBlock();
	void EndBlock();
	
private:
	bool bGuard = false;
	bool bParrying = false;
	FTimerHandle ParryingTimer;

/* 스킬 */
public:
	/* Q 스킬 */
	void Begin_Q();
	void End_Q(class UAnimMontage* Target, bool IsProperlyEnded);
	virtual void Q_SkillHitCheck() override;

	/* E 스킬 */
	void Begin_E();
	void End_E(class UAnimMontage* Target, bool IsProperlyEnded);
	bool CheckInSkillE(FHitResult& InHitResult);
	void ESkillMotionWarpSet();

	/* R 스킬 */
	void Begin_R();
	void End_R(class UAnimMontage* Target, bool IsProperlyEnded);

	/* 패링 성공시 공격 */
	void BeginParryingAttack();
	void EndParryingAttack(class UAnimMontage* Target, bool IsProperlyEnded);
	void StartEnemyParryingAttack(AActor* InActor);



/* 몽타주 */
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> ProgressAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> Q_Montage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> E_Montage_Fail;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> E_Montage_Success;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> R_Montage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> ParryingAttackMontage;

/* 유틸리티 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Character")
	TObjectPtr<class ACharacter> Character;

	class UMotionWarpingComponent* GetMotionWarpComponent(uint8 Index);
	class APlayerController* GetPlayerController();
};
