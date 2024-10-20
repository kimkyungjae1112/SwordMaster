// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveAndWatchPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMASTER_API UBTTask_MoveAndWatchPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveAndWatchPlayer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
