// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Patrol.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIInterface.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* Navigation = UNavigationSystemV1::GetNavigationSystem(Owner->GetWorld());
	if (Navigation == nullptr) return EBTNodeResult::Failed;

	IAIInterface* Interface = Cast<IAIInterface>(Owner);
	if (Interface == nullptr) return EBTNodeResult::Failed;

	FVector Origin = Owner->GetActorLocation();
	FNavLocation NextPatrolPos;
	float Radius = Interface->GetPatrolRadius();

	if (Navigation->GetRandomPointInNavigableRadius(Origin, Radius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPos"), NextPatrolPos);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
