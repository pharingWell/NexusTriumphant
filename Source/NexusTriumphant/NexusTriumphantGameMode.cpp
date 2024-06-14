// Copyright Epic Games, Inc. All Rights Reserved.

#include "NexusTriumphantGameMode.h"
#include "Characters/Base/BasePlayerController.h"
#include "Characters/Base/BaseCharacter.h"
#include "Abilities/AbilityHelpers.h"
#include "UObject/ConstructorHelpers.h"

ANexusTriumphantGameMode::ANexusTriumphantGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABasePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/NexusTriumphant/Characters/Blueprints/BP_BaseCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		//UE_LOG_ABILITY_CAUTION("Default Pawn used", this);
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/NexusTriumphant/Characters/Blueprints/BP_BasePlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}