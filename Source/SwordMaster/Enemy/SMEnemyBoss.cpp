// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SMEnemyBoss.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Enemy/EnemyHitData.h"
#include "Enemy/EnemyDefaultAttackData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/Controller/AIControllerBoss.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"

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

	/* Motion Warping */
	MotionWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion Warping"));
	ParryingHitWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Parrying Hit Warping"));
}

void ASMEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	MyController = CastChecked<AAIControllerBoss>(GetController());
	Player = MyController->GetPlayer();
}

float ASMEnemyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	return DamageAmount;
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
	if (CurrentCombo == 1 && bCanAttack)
	{
		BeginDefaultAttack();
		bCanAttack = false;
		return;
	}

}

void ASMEnemyBoss::AttackEndTiming()
{
	TArray<FOverlapResult> OverlapResults;
	if (!MeleeAttackDetectInRange(OverlapResults))
	{
		OnAttackFinished.ExecuteIfBound();
	}
}

void ASMEnemyBoss::DefaultAttackHitCheck()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	TArray<FOverlapResult> OverlapResults;
	if (MeleeAttackDetectInRange(OverlapResults))
	{
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, DefaultAttackData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *DefaultAttackData->MontageSectionNamePrefix, CurrentCombo);
		Anim->Montage_JumpToSection(NextSection, DefaultAttackMontage);
		UE_LOG(LogTemp, Display, TEXT("현재 몽타주 섹션 : %d"), CurrentCombo);
	}
}

void ASMEnemyBoss::BeginParryingAttackHit()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	MyController->StopAI();
	MyController->SetMoveFlag(false);
	
	ParryingAttackHitMotionWarpSet();
	AnimInstance->Montage_Play(ParryingAttackHitMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &ASMEnemyBoss::EndParryingAttackHit);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, ParryingAttackHitMontage);
}

void ASMEnemyBoss::EndParryingAttackHit(UAnimMontage* Target, bool IsProperlyEnded)
{
	MyController->RunAI();
	MyController->SetMoveFlag(true);
}

void ASMEnemyBoss::ParryingAttackHitMotionWarpSet()
{
	TArray<FOverlapResult> OverlapResults;
	if (MeleeAttackDetectInRange(OverlapResults))
	{
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			AActor* Target = OverlapResult.GetActor();
		
			FVector PlayerLoc = GetActorLocation();
			FVector TargetLoc = Target->GetActorLocation();
			FVector MoveLoc = -GetActorForwardVector() * 100.f + GetActorLocation();
			FRotator TargetRotator = UKismetMathLibrary::MakeRotFromX(TargetLoc - PlayerLoc);

			ParryingHitWarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("Parrying"), MoveLoc, TargetRotator);
		}
	}
}

void ASMEnemyBoss::BeginDefaultAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	DefaultAttackMotionWarpSet();
	AnimInstance->Montage_Play(DefaultAttackMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &ASMEnemyBoss::EndDefaultAttack);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, DefaultAttackMontage);
}

void ASMEnemyBoss::EndDefaultAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	CurrentCombo = 1;
	bCanAttack = true;
	MotionWarpComp->RemoveAllWarpTargets();
}

void ASMEnemyBoss::DefaultAttackMotionWarpSet()
{
	FVector Origin = GetActorLocation();
	FVector TargetLoc = Origin + GetActorForwardVector() * 100.f;

	MotionWarpComp->AddOrUpdateWarpTargetFromLocation(TEXT("DefaultAttack"), TargetLoc);
}

bool ASMEnemyBoss::MeleeAttackDetectInRange(TArray<FOverlapResult>& InOverlapResults)
{
	FVector Origin = GetActorLocation();
	FCollisionQueryParams Params(NAME_None, false, this);
	return GetWorld()->OverlapMultiByChannel(InOverlapResults, Origin, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(GetAttackRange()), Params);
}





