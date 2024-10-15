// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SMCharacterBase.h"

ASMCharacterBase::ASMCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

void ASMCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASMCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
