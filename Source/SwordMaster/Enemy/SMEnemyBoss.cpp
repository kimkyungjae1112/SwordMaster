// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SMEnemyBoss.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Enemy/EnemyHitData.h"
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
	BeginDefaultAttack();
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

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(DefaultAttackMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &ASMEnemyBoss::EndDefaultAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, DefaultAttackMontage);
}

void ASMEnemyBoss::EndDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}


