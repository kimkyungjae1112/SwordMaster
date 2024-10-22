// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/BossAttackHitCheckNotify.h"
#include "Interface/AIInterface.h"

UBossAttackHitCheckNotify::UBossAttackHitCheckNotify()
{
}

void UBossAttackHitCheckNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAIInterface* Interface = Cast<IAIInterface>(MeshComp->GetOwner());
	if (Interface)
	{
		Interface->DefaultAttackHitCheck();
	}
}
