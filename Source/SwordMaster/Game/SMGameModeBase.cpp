// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SMGameModeBase.h"
#include "Player/SMPlayerController.h"

ASMGameModeBase::ASMGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnRef(TEXT("/Game/SwordMaster/Blueprint/BP_SMCharacter.BP_SMCharacter_C"));
	if (DefaultPawnRef.Class)
	{
		DefaultPawnClass = DefaultPawnRef.Class;
	}
	static ConstructorHelpers::FClassFinder<ASMPlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/SwordMaster.SMPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
