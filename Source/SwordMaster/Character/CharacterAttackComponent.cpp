// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/CharacterAnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Character/CharacterProgressAttackData.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionWarpingComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/PlayerController.h"

UCharacterAttackComponent::UCharacterAttackComponent()
{
	static ConstructorHelpers::FObjectFinder<UCharacterProgressAttackData> ProgressAttackDataRef(TEXT("/Script/SwordMaster.CharacterProgressAttackData'/Game/SwordMaster/Character/Data/DA_ProgressAttackData.DA_ProgressAttackData'"));
	if (ProgressAttackDataRef.Object)
	{
		ProgressAttackData = ProgressAttackDataRef.Object;
	}
}


void UCharacterAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = CastChecked<ACharacter>(GetOwner());
}

/* 콤보 어택 */
void UCharacterAttackComponent::ProgressAttack()
{
	if (CurrentPA == 0)
	{
		BeginProgressAttack();
		return;
	}

	if (!PATimerHandle.IsValid())
	{
		HasNextPACommand = false;
	}
	else
	{
		HasNextPACommand = true;
	}
}

void UCharacterAttackComponent::BeginProgressAttack()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	CurrentPA = 1;

	ProgressAttackTargetSet();
	AnimInstance->Montage_Play(ProgressAttackMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UCharacterAttackComponent::EndProgressAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, ProgressAttackMontage);

	PATimerHandle.Invalidate();
	SetProgressAttackTimer();
}

void UCharacterAttackComponent::EndProgressAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	ensure(CurrentPA != 0);
	CurrentPA = 0;
	ProgressAttackRemoveTarget();
}

void UCharacterAttackComponent::SetProgressAttackTimer()
{
	int32 ComboIndex = CurrentPA - 1;
	ensure(ProgressAttackData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = (ProgressAttackData->EffectiveFrameCount[ComboIndex] / ProgressAttackData->FrameRate);
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(PATimerHandle, this, &UCharacterAttackComponent::ProgressAttackCheck, ComboEffectiveTime, false);
	}
}

void UCharacterAttackComponent::ProgressAttackCheck()
{
	PATimerHandle.Invalidate();
	if (HasNextPACommand)
	{
		UAnimInstance* AnimInstacne = Character->GetMesh()->GetAnimInstance();

		CurrentPA = FMath::Clamp(CurrentPA + 1, 1, ProgressAttackData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ProgressAttackData->MontageSectionNamePrefix, CurrentPA);
		ProgressAttackTargetSet();
		AnimInstacne->Montage_JumpToSection(NextSection, ProgressAttackMontage);

		SetProgressAttackTimer();
		HasNextPACommand = false;
	}
}



/* 모션 워핑 */
void UCharacterAttackComponent::ProgressAttackTargetSet()
{
	TArray<FOverlapResult> OverlapResults;
	if (ProgressAttackSphereCheck(OverlapResults))
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			if(ProgressAttackInDegree(OverlapResult.GetActor(), 90.f))
			{
				const FVector PlayerLocation = Character->GetActorLocation();
				const FVector TargetLocation = OverlapResult.GetActor()->GetActorLocation();
				const FVector MoveLocation = TargetLocation + (PlayerLocation - TargetLocation).GetSafeNormal() * 50.f;
				const FVector Direction = (TargetLocation - PlayerLocation).GetSafeNormal();
				
				FRotator TargetRotator = UKismetMathLibrary::MakeRotFromX(Direction);
				GetMotionWarpComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("ProgressAttack"), MoveLocation, TargetRotator);

				return;
			}	
		}
	}
	else
	{
		GetMotionWarpComponent()->RemoveWarpTarget(TEXT("ProgressAttack"));
	}
}

void UCharacterAttackComponent::ProgressAttackRemoveTarget()
{
	GetMotionWarpComponent()->RemoveWarpTarget(TEXT("ProgressAttack"));
}


/* 모션 워핑, 히트체크에 필요한 함수들 */
bool UCharacterAttackComponent::ProgressAttackSphereCheck(TArray<FOverlapResult>& OverlapResults)
{
	const float Range = 300.f;
	
	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams Params(NAME_None, false, Character);
	FColor Color = FColor::Red;

	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Range), Params);

	if (bHit)
	{
		// OverlapResults 배열을 거리 기준으로 정렬
		OverlapResults.Sort([this](const FOverlapResult& A, const FOverlapResult& B)
			{
				// A와 B 액터 간의 거리를 계산
				float DistanceA = FVector::Dist(Character->GetActorLocation(), A.GetActor()->GetActorLocation());
				float DistanceB = FVector::Dist(Character->GetActorLocation(), B.GetActor()->GetActorLocation());

				// 오름차순으로 정렬
				return DistanceA < DistanceB;
			});
		Color = FColor::Green;
	}
	
	return bHit;
}

bool UCharacterAttackComponent::ProgressAttackInDegree(AActor* Actor, float Degree)
{
	FVector PlayerLocation = Character->GetActorLocation();
	FVector ForwardVector = Character->GetActorForwardVector();
	FVector TargetLocation = Actor->GetActorLocation();
	FVector Direction = (TargetLocation - PlayerLocation).GetSafeNormal();

	float DotProduct = FVector::DotProduct(ForwardVector, Direction);
	float AngleToTarget = FMath::Acos(DotProduct);

	float AngleToTargetDegrees = FMath::RadiansToDegrees(AngleToTarget);

	return AngleToTargetDegrees <= (Degree / 2.f);
}

void UCharacterAttackComponent::ProgressAttackHitCheck()
{
	const float Damage = 50.f;
	TArray<FOverlapResult> OverlapResults;
	if (ProgressAttackSphereCheck(OverlapResults))
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			if (ProgressAttackInDegree(OverlapResult.GetActor(), 90.f))
			{
				FDamageEvent DamageEvent;
				OverlapResult.GetActor()->TakeDamage(Damage, DamageEvent, GetPlayerController(), Character);
			}
		}
	}
}

void UCharacterAttackComponent::BeginBlock()
{
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance());

	AnimInstance->bIsBlock = true;
}

void UCharacterAttackComponent::EndBlock()
{
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance());

	AnimInstance->bIsBlock = false;
}

UMotionWarpingComponent* UCharacterAttackComponent::GetMotionWarpComponent()
{
	return Character->GetComponentByClass<UMotionWarpingComponent>();
}

APlayerController* UCharacterAttackComponent::GetPlayerController()
{
	return Cast<APlayerController>(Character->GetController());
}



