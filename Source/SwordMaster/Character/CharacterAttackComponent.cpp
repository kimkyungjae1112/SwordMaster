// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Character/CharacterProgressAttackData.h"

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

void UCharacterAttackComponent::ProgressAttack()
{
	if (CurrentPA == 0)
	{
		BeginProgressAttack();
		UE_LOG(LogTemp, Display, TEXT("실행되나?"));
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
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
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

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
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
		AnimInstacne->Montage_JumpToSection(NextSection, ProgressAttackMontage);

		SetProgressAttackTimer();
		HasNextPACommand = false;
	}
}



