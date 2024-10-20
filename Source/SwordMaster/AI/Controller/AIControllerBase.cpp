// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/AIControllerBase.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAIControllerBase::AAIControllerBase()
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

void AAIControllerBase::RunAI()
{
	UBlackboardComponent* BBComp = Blackboard.Get();
	if (UseBlackboard(BBData, BBComp))
	{
		BBComp->SetValueAsVector(TEXT("SpawnPos"), GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTData);
	}
}

void AAIControllerBase::StopAI()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp)
	{
		BTComp->StopTree();
	}
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
