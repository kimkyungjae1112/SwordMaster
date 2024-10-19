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
	BeginProgressAttackHit();

	return DamageAmount;
}

void ASMEnemyBoss::BeginProgressAttackHit()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (CurrentHit >= 1 && AnimInstance->Montage_IsPlaying(ProgressAttackHitData->ProgressAttackHitMontages[CurrentHit - 1]))
	{

	}

	int32 Temp = CurrentHit;
	AnimInstance->Montage_Play(ProgressAttackHitData->ProgressAttackHitMontages[Temp]);
	CurrentHit = FMath::Clamp(CurrentHit + 1, 1, 3);
	
	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &ASMEnemyBoss::EndProgressAttackHit);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, ProgressAttackHitData->ProgressAttackHitMontages[Temp]);
}

void ASMEnemyBoss::EndProgressAttackHit(UAnimMontage* Target, bool IsProperlyEnded)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(ProgressAttackHitData->ProgressAttackHitMontages[CurrentHit - 1]))
	{
		if (CurrentHit == 3) CurrentHit = 0;
	}
	else
	{
		CurrentHit = 0;
	}
}


