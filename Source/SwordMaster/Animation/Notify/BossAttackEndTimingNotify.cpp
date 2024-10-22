// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/BossAttackEndTimingNotify.h"
#include "Interface/AIInterface.h"

UBossAttackEndTimingNotify::UBossAttackEndTimingNotify()
{
}

void UBossAttackEndTimingNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAIInterface* Interface = Cast<IAIInterface>(MeshComp->GetOwner());
	if (Interface)
	{
		Interface->AttackEndTiming();
	}

}
