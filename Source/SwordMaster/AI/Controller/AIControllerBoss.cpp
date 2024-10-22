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

void AAIControllerBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* Target = Cast<APawn>(GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target)
	{
		FRotator TargetRotation = (Target->GetActorLocation() - GetPawn()->GetActorLocation()).Rotation();
		TargetRotation.Roll = 0;
		TargetRotation.Pitch = 0;

		Interval += DeltaTime;
		if (Interval >= 1.5f)
		{
			FVector TargetLocation = Target->GetActorLocation();
			TargetLocation.X += FMath::RandRange(-300.f, 300.f);
			TargetLocation.Y += FMath::RandRange(-300.f, 300.f);
			MoveToLocation(TargetLocation);
			Interval = 0.f;
		}
		GetPawn()->SetActorRotation(TargetRotation);
	}
}

