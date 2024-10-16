// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterParkourComponent.h"
#include "GameFramework/Character.h"
#include "Animation/CharacterAnimInstance.h"

UCharacterParkourComponent::UCharacterParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCharacterParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = CastChecked<ACharacter>(GetOwner());
}


void UCharacterParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCharacterParkourComponent::BeginCrouch()
{
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance());

	AnimInstance->bIsCrouch = true;
}

void UCharacterParkourComponent::EndCrouch()
{
	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance());

	AnimInstance->bIsCrouch = false;
}

