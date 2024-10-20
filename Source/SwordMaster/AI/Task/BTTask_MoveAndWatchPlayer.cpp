// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_MoveAndWatchPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIInterface.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_MoveAndWatchPlayer::UBTTask_MoveAndWatchPlayer()
{
	NodeName = TEXT("MoveAndWatchPlayer");
}

EBTNodeResult::Type UBTTask_MoveAndWatchPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return EBTNodeResult::Failed;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Owner->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	float DistanceToPlayer = FVector::Dist(Owner->GetActorLocation(), Target->GetActorLocation());
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), Target->GetActorLocation());
	//Owner->SetActorRotation(LookAtRotation);

	/*if (DistanceToPlayer >= 400.f)
	{
		OwnerComp.GetAIOwner()->MoveToActor(Target);
	}
	else
	{
		FNavLocation NavLocation;
		if (NavSystem->GetRandomPointInNavigableRadius(Owner->GetActorLocation(), 400.f, NavLocation))
		{
			OwnerComp.GetAIOwner()->MoveToLocation(NavLocation);
			float SuccessDistance = FVector::Dist(NavLocation, Owner->GetActorLocation());
			if (SuccessDistance <= 50.f)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}*/

	return EBTNodeResult::Succeeded;
}
