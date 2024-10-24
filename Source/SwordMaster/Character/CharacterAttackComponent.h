// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttackHitCheckInterface.h"
#include "CharacterAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDMASTER_API UCharacterAttackComponent : public UActorComponent, public IAttackHitCheckInterface
{
	GENERATED_BODY()

public:	
	UCharacterAttackComponent();

protected:
	virtual void BeginPlay() override;

	
	
/* 공격 */
public:
	void ProgressAttack();

private:
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
	void BeginBlock();
	void EndBlock();


/* 몽타주 */
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> ProgressAttackMontage;


/* 유틸리티 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Character")
	TObjectPtr<class ACharacter> Character;

	class UMotionWarpingComponent* GetMotionWarpComponent();
	class APlayerController* GetPlayerController();
};
