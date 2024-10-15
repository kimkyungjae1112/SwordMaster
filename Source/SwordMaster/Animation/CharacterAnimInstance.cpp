// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	MovingThreshould = 3.f;
	JumpingThreshould = 30.f;
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());
	if (Character)
	{
		Movement = Character->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size();
		Angle = UKismetMathLibrary::InverseTransformDirection(Character->GetActorTransform(), Velocity);
		bIsIdle = MovingThreshould > GroundSpeed;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}
