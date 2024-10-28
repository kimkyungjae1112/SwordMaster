// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SMCharacterBase.h"
#include "InputActionValue.h"
#include "SMCharacter.generated.h"

class ASMPlayerController;
/**
 * 
 */
UCLASS()
class SWORDMASTER_API ASMCharacter : public ASMCharacterBase
{
	GENERATED_BODY()
	
public:
	ASMCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


/* 카메라 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> Camera;

/* 입력 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputMappingContext> IMC;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> LookUpAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> BlockAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> CrouchAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> DashAction;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> Q_Action;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> E_Action;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> R_Action;

	void Move(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	
	/* 공격 */
	void Attack();
	
	/* 방어 */
	void BeginParrying();
	void BeginBlock();
	void EndBlock();
	bool bGaurd = false;

	/* 파쿠르 */
	void BeginCrouch();
	void EndCrouch();
	void BeginRun();
	void StopRun();
	void BeginDash();

	/* 스킬 */
	void Begin_Q();
	void Begin_E();
	void Begin_R();

/* 공격 컴포넌트 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	TObjectPtr<class UCharacterAttackComponent> AttackComponent;

/* 무기 */
private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<class USkeletalMeshComponent> SwordComponent;

/* 모션 워핑 */
private:
	UPROPERTY(VisibleAnywhere, Category = "MotionWarp")
	TObjectPtr<class UMotionWarpingComponent> MotionWarpComponent;

	UPROPERTY(VisibleAnywhere, Category = "MotionWarp")
	TObjectPtr<class UMotionWarpingComponent> ParryingWarpComponent;

	UPROPERTY(VisibleAnywhere, Category = "MotionWarp")
	TObjectPtr<class UMotionWarpingComponent> ESkillWarpComponent;


/* 파쿠르 컴포넌트 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Parkour")
	TObjectPtr<class UCharacterParkourComponent> ParkourComponent;


private:
	ASMPlayerController* GetPlayerController();

};
