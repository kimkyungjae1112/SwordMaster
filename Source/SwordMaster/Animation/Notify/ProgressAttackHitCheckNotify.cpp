// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/ProgressAttackHitCheckNotify.h"
#include "Character/CharacterAttackComponent.h"
#include "Interface/AttackHitCheckInterface.h"

UProgressAttackHitCheckNotify::UProgressAttackHitCheckNotify()
{
}

void UProgressAttackHitCheckNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAttackHitCheckInterface* Interface = Cast<IAttackHitCheckInterface>(MeshComp->GetOwner()->GetComponentByClass<UCharacterAttackComponent>());
	if (Interface)
	{
		Interface->ProgressAttackHitCheck();
	}
}
