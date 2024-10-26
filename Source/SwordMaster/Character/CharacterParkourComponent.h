// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterParkourComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SWORDMASTER_API UCharacterParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterParkourComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void BeginCrouch();
	void EndCrouch();

	void BeginEvade();
	void EndEvade(class UAnimMontage* Target, bool IsProperlyEnded);

	
private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<class UAnimMontage> EvadeMontage;

private:
	UPROPERTY(VisibleAnywhere, Category = "Util")
	TObjectPtr<class ACharacter> Character;
};
