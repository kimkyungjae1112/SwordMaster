// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Q_SkillHitCheckNotify.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMASTER_API UQ_SkillHitCheckNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UQ_SkillHitCheckNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
