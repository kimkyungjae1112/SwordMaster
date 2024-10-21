// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SMEnemyBoss.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Enemy/EnemyHitData.h"
#include "Enemy/EnemyDefaultAttackData.h"
#include "GameFramework/CharacterMovementComponent.h"

ASMEnemyBoss::ASMEnemyBoss()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DF_DRAGON_KNIGHT/MESHES/UE4/SK_Dragon_knight_UE4.SK_Dragon_knight_UE4'"));
	if (BodyMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(BodyMeshRef.Object);
	}
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/DF_DRAGON_KNIGHT/MESHES/SWORD/SK_Dragon_knight_sword.SK_Dragon_knight_sword'"));
	if (WeaponMeshRef.Object)
	{
		WeaponMeshComp->SetSkeletalMesh(WeaponMeshRef.Object);
	}

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

float ASMEnemyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	BeginProgressAttackHit();

	return DamageAmount;
}

float ASMEnemyBoss::GetDetectRadius()
{
	return 800.f;
}

float ASMEnemyBoss::GetPatrolRadius()
{
	return 600.0f;
}

float ASMEnemyBoss::GetAttackRange()
{
	return 300.0f;
}

void ASMEnemyBoss::SetAttackFinished(const FOnAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ASMEnemyBoss::AttackByAI()
{
	if (DefaultAttackCombo == 0)
	{
		BeginDefaultAttack();
		return;
	}

	if (!DefaultAttackTimer.IsValid())
	{
		HasNextCombo = false;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("AttackCombo"));
		HasNextCombo = true;
	}
}

void ASMEnemyBoss::BeginProgressAttackHit()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	int32 Temp = CurrentHit;
	AnimInstance->Montage_Play(ProgressAttackHitData->ProgressAttackHitMontages[Temp]);
	CurrentHit = FMath::Clamp(CurrentHit + 1, 1, 3);
	HasNextHit1 = true;

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &ASMEnemyBoss::EndProgressAttackHit);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, ProgressAttackHitData->ProgressAttackHitMontages[Temp]);
}

void ASMEnemyBoss::EndProgressAttackHit(UAnimMontage* Target, bool IsProperlyEnded)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!HasNextHit1)
	{
		CurrentHit = 0;
	}
	else
	{
		HasNextHit1 = false;
	}
}

void ASMEnemyBoss::BeginDefaultAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	DefaultAttackCombo = 1;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(DefaultAttackMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &ASMEnemyBoss::EndDefaultAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, DefaultAttackMontage);

	DefaultAttackTimer.Invalidate();
	OnAttackFinished.ExecuteIfBound();
	SetDefaultAttackTimer();
}

void ASMEnemyBoss::EndDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	ensure(DefaultAttackCombo != 0);
	DefaultAttackCombo = 0;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}

void ASMEnemyBoss::SetDefaultAttackTimer()
{
	int32 Combo = DefaultAttackCombo - 1;
	ensure(DefaultAttackData->EffectiveFrameCount.IsValidIndex(Combo));

	float EffectiveTime = (DefaultAttackData->EffectiveFrameCount[Combo] / DefaultAttackData->FrameRate);
	if (EffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(DefaultAttackTimer, this, &ASMEnemyBoss::DefaultAttackComboCheck, EffectiveTime, false);
	}
}

void ASMEnemyBoss::DefaultAttackComboCheck()
{
	UE_LOG(LogTemp, Display, TEXT("ComboCheck"));
	DefaultAttackTimer.Invalidate();
	if (HasNextCombo)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		DefaultAttackCombo = FMath::Clamp(DefaultAttackCombo + 1, 1, DefaultAttackData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *DefaultAttackData->MontageSectionNamePrefix, DefaultAttackCombo);
		AnimInstance->Montage_JumpToSection(NextSection, DefaultAttackMontage);

		OnAttackFinished.ExecuteIfBound();
		SetDefaultAttackTimer();
		HasNextCombo = false;
	}
}


