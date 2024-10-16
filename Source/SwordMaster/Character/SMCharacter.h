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

	void Move(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Attack();
	void BeginBlock();
	void EndBlock();

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
	UPROPERTY(EditAnywhere, Category = "MotionWarp")
	TObjectPtr<class UMotionWarpingComponent> MotionWarpComponent;


private:
	ASMPlayerController* GetPlayerController();

};
