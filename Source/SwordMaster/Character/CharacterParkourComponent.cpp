// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterParkourComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
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

void UCharacterParkourComponent::BeginEvade()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(EvadeMontage);
	FVector Velocity = Character->GetActorForwardVector() * 2500;
	Character->LaunchCharacter(Velocity, true, false);
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UCharacterParkourComponent::EndEvade);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, EvadeMontage);
}

void UCharacterParkourComponent::EndEvade(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

