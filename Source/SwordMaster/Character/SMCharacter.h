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

	void Move(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);


private:
	ASMPlayerController* GetPlayerController();

};
