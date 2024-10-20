// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/AIControllerBoss.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIControllerBoss::AAIControllerBoss()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBDataRef(TEXT("/Script/AIModule.BlackboardData'/Game/SwordMaster/AI/Boss/BB_Boss.BB_Boss'"));
	if (BBDataRef.Object)
	{
		BBData = BBDataRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTDataRef(TEXT("/Script/AIModule.BehaviorTree'/Game/SwordMaster/AI/Boss/BT_Boss.BT_Boss'"));
	if (BTDataRef.Object)
	{
		BTData = BTDataRef.Object;
	}
}

