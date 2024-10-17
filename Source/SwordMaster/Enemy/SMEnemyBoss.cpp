// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SMEnemyBoss.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Enemy/EnemyHitData.h"

ASMEnemyBoss::ASMEnemyBoss()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DF_DRAGON_KNIGHT/MESHES/UE4/SK_Dragon_knight_UE4.SK_Dragon_knight_UE4'"));
	if (BodyMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(BodyMeshRef.Object);
	}
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
}

float ASMEnemyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("Damaged Amount : %f"), DamageAmount);
	if (Hit1Count == 0)
	{
		BeginHit1();
		return -1.f;
	}
	
	//Isvalid = 0이 아니면 true
	//Timer의 Handle 이 0이면, 즉 초기화 상태면
	if (!Hit1TimerHandle.IsValid())
	{
		HasNextHitAction = false;
	}
	else
	{
		HasNextHitAction = true;
	}

	return DamageAmount;
}

void ASMEnemyBoss::BeginHit1()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	Hit1Count = 1;
	UE_LOG(LogTemp, Display, TEXT("Hit1Count : %d"), Hit1Count);

	AnimInstance->Montage_Play(Hit1Montage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &ASMEnemyBoss::EndHit1);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, Hit1Montage);

	Hit1TimerHandle.Invalidate();
	SetTimerHit1();
}

void ASMEnemyBoss::EndHit1(UAnimMontage* Target, bool IsProperlyEnded)
{
	ensure(Hit1Count != 0);
	Hit1Count = 0;
}

void ASMEnemyBoss::SetTimerHit1()
{
	int32 HitCount = Hit1Count - 1;
	ensure(HitData->EffectiveFrameCount.IsValidIndex(HitCount));

	float HitEffectiveTime = (HitData->EffectiveFrameCount[HitCount] / HitData->FrameRate);
	if (HitEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(Hit1TimerHandle, this, &ASMEnemyBoss::Hit1TimerCheck, HitEffectiveTime, false);
	}
}

void ASMEnemyBoss::Hit1TimerCheck()
{
	Hit1TimerHandle.Invalidate();
	if (HasNextHitAction)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		Hit1Count = FMath::Clamp(Hit1Count + 1, 1, HitData->MaxHitCount);
		UE_LOG(LogTemp, Display, TEXT("Hit1TimerCheck() : %d"), Hit1Count );
		FName NextSection = *FString::Printf(TEXT("%s%d"), *HitData->MontageSectionNamePrefix, Hit1Count);
		UE_LOG(LogTemp, Display, TEXT("Name : %s"), *NextSection.ToString());
		AnimInstance->Montage_JumpToSection(NextSection, Hit1Montage);

		SetTimerHit1();
		HasNextHitAction = false;
	}
}
