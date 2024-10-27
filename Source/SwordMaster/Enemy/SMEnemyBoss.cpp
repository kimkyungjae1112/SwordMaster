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
}

float ASMEnemyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

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

void ASMEnemyBoss::AttackEndTiming()
{
	TArray<FOverlapResult> OverlapResults;
	if (!DetectInRange(OverlapResults))
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		AnimInstance->StopAllMontages(0.5f);
	}
}

void ASMEnemyBoss::DefaultAttackHitCheck()
{
	float Damage = 50.f;

	TArray<FOverlapResult> OverlapResults;
	FColor Color = FColor::Red;

	if (DetectInRange(OverlapResults))
	{
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			FDamageEvent DamageEvent;
			OverlapResult.GetActor()->TakeDamage(Damage, DamageEvent, GetController(), this);
			Color = FColor::Green;
		}
	}
	
	FVector Origin = GetActorLocation();
	DrawDebugSphere(GetWorld(), Origin, GetAttackRange(), 24, Color, false, 3.f);
}

void ASMEnemyBoss::BeginProgressAttackHit()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	MyController->StopAI();

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
	if (!HasNextHit1)
	{
		MyController->RunAI();
		CurrentHit = 0;
	}
	else
	{
		HasNextHit1 = false;
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
	if (DetectInRange(OverlapResults))
	{
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			AActor* Target = OverlapResult.GetActor();
		
			const FVector PlayerLoc = GetActorLocation();
			const FVector TargetLoc = Target->GetActorLocation();
			const FVector MoveLoc = -GetActorForwardVector() * 100.f + GetActorLocation();
			const FRotator TargetRotator = UKismetMathLibrary::MakeRotFromX(TargetLoc - PlayerLoc);

			UE_LOG(LogTemp, Display, TEXT("Parrying Hit MotionWarp"));
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
	OnAttackFinished.ExecuteIfBound();
}

void ASMEnemyBoss::DefaultAttackMotionWarpSet()
{
	const FVector PlayerLoc = GetActorLocation();
	const FVector TargetLoc = PlayerLoc + GetActorForwardVector() * 100.f;
	const FRotator TargetRotator = UKismetMathLibrary::MakeRotFromX(TargetLoc - PlayerLoc);

	MotionWarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("DefaultAttack"), TargetLoc, TargetRotator);
}

bool ASMEnemyBoss::DetectInRange(TArray<FOverlapResult>& InOverlapResults)
{
	FVector Origin = GetActorLocation();
	FCollisionQueryParams Params(NAME_None, false, this);
	return GetWorld()->OverlapMultiByChannel(InOverlapResults, Origin, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(GetAttackRange()), Params);
}





