// Copyright Epic Games, Inc. All Rights Reserved.

#include "NexusTriumphantGameMode.h"
#include "Public/Player/NPlayerController.h"
#include "AbilitySystem/Abilities/NAbilityHelpers.h"
#include "Player/NPlayerController.h"
#include "Player/NPlayerState.h"
#include "UObject/ConstructorHelpers.h"

ANexusTriumphantGameMode::ANexusTriumphantGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ANPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NexusTriumphant/Player/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		//UE_LOG_ABILITY_CAUTION("Default Pawn used", this);
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/NexusTriumphant/Player/BP_BasePlayerController"));
	if(PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	PlayerStateClass = ANPlayerState::StaticClass();
}
