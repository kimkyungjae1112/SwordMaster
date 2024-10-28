// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/Q_SkillHitCheckNotify.h"
#include "Interface/AttackHitCheckInterface.h"
#include "Character/CharacterAttackComponent.h"

UQ_SkillHitCheckNotify::UQ_SkillHitCheckNotify()
{
}

void UQ_SkillHitCheckNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAttackHitCheckInterface* Interface = Cast<IAttackHitCheckInterface>(MeshComp->GetOwner()->GetComponentByClass<UCharacterAttackComponent>());
	if (Interface)
	{
		Interface->Q_SkillHitCheck();
	}
}
