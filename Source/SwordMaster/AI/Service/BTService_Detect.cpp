// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_Detect.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIInterface.h"
#include "Engine/OverlapResult.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return;

	IAIInterface* Interface = Cast<IAIInterface>(Owner);
	if (Interface == nullptr) return;

	float Radius = Interface->GetDetectRadius();
	TArray<FOverlapResult> OverlapResults;
	FVector Origin = Owner->GetActorLocation();
	FCollisionQueryParams Params(NAME_None, false, Owner);

	bool bDetect = Owner->GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Radius), Params);
	if (bDetect)
	{
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			APawn* Target = Cast<APawn>(OverlapResult.GetActor());
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Target);
		
			float Dist = FVector::Dist(Target->GetActorLocation(), Owner->GetActorLocation());
			if (Dist <= Interface->GetAttackRange())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), true);
			}
			else
			{

				OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), false);
			}
		}

		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	return;
}
