// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SMCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Player/SMPlayerController.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Character/CharacterProgressAttackData.h"
#include "Character/CharacterAttackComponent.h"
#include "Character/CharacterParkourComponent.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASMCharacter::ASMCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 650.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;

	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	SwordComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	SwordComponent->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	/* 컴포넌트 */
	AttackComponent = CreateDefaultSubobject<UCharacterAttackComponent>(TEXT("Attack Component"));
	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	ParkourComponent = CreateDefaultSubobject<UCharacterParkourComponent>(TEXT("Parkour Component"));

	/* Input */
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/SwordMaster/Input/IMC/IMC_Default.IMC_Default'"));
	if (IMCRef.Object)
	{
		IMC = IMCRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SwordMaster/Input/IA/IA_Move.IA_Move'"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> LookUpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SwordMaster/Input/IA/IA_LookUp.IA_LookUp'"));
	if (LookUpActionRef.Object)
	{
		LookUpAction = LookUpActionRef.Object;
	}	
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SwordMaster/Input/IA/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SwordMaster/Input/IA/IA_Attack.IA_Attack'"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> BlockActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SwordMaster/Input/IA/IA_Block.IA_Block'"));
	if (BlockActionRef.Object)
	{
		BlockAction = BlockActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> CrouchActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SwordMaster/Input/IA/IA_Crouch.IA_Crouch'"));
	if (CrouchActionRef.Object)
	{
		CrouchAction = CrouchActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> RunActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SwordMaster/Input/IA/IA_Run.IA_Run'"));
	if (RunActionRef.Object)
	{
		RunAction = RunActionRef.Object;
	}
}

void ASMCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(IMC, 0);
	}
}

void ASMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASMCharacter::Move);
	EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ASMCharacter::LookUp);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASMCharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASMCharacter::StopJumping);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASMCharacter::Attack);
	EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &ASMCharacter::BeginBlock);
	EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ASMCharacter::EndBlock);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ASMCharacter::BeginCrouch);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ASMCharacter::EndCrouch);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ASMCharacter::BeginRun);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASMCharacter::StopRun);

}

void ASMCharacter::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InputValue.X);
	AddMovementInput(RightDirection, InputValue.Y);
}

void ASMCharacter::LookUp(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	AddControllerPitchInput(InputValue.X * 0.5f);
	AddControllerYawInput(-InputValue.Y * 0.5f);
}

void ASMCharacter::Attack()
{
	AttackComponent->ProgressAttack();
}

void ASMCharacter::BeginBlock()
{
	AttackComponent->BeginBlock();
}

void ASMCharacter::EndBlock()
{
	AttackComponent->EndBlock();
}

void ASMCharacter::BeginCrouch()
{
	ParkourComponent->BeginCrouch();
}

void ASMCharacter::EndCrouch()
{
	ParkourComponent->EndCrouch();
}

void ASMCharacter::BeginRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void ASMCharacter::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
}



ASMPlayerController* ASMCharacter::GetPlayerController()
{
	return CastChecked<ASMPlayerController>(GetController());
}
