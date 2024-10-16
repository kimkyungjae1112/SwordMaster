// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SMEnemyBoss.h"
#include "Components/CapsuleComponent.h"

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
