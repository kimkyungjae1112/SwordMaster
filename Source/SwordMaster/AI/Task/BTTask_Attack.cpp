// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return EBTNodeResult::Failed;

	IAIInterface* Interface = Cast<IAIInterface>(Owner);
	if (Interface == nullptr) return EBTNodeResult::Failed;

	FOnAttackFinished OnAttackFinished;
	/*OnAttackFinished.BindLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});

	Interface->SetAttackFinished(OnAttackFinished);*/
	Interface->AttackByAI();

	return EBTNodeResult::Succeeded;
}
