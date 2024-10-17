// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ProgressAttackHitCheckNotify.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMASTER_API UProgressAttackHitCheckNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UProgressAttackHitCheckNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
