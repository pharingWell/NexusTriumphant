// Copyright Epic Games, Inc. All Rights Reserved.

#include "Entities/Player/NPlayerController.h"

#include "Entities/NexusEntity.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Entities/Player/NPlayerCharacter.h"

DEFINE_LOG_CATEGORY(LogBaseEntity);

ANPlayerController::ANPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	ExecuteActionComponent = CreateDefaultSubobject<UNExecuteActionComponent>(TEXT("ExecuteActionComponent"));
}


void ANPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

void ANPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	ANPlayerState* NPlayerState = GetPlayerState<ANPlayerState>();
	if (ANPlayerCharacter* CharacterBase = Cast<ANPlayerCharacter>(P))
	{
		if(NPlayerState)
		{
			NPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(NPlayerState, CharacterBase);
			CharacterBase->SetPlayerState(NPlayerState);
			ExecuteActionComponent->SetPlayerState(NPlayerState);
		}
		
	}

	//...
}

void ANPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	//GetPlayerState<>()
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Started, this, &ANPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Triggered, this, &ANPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Completed, this, &ANPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Canceled, this, &ANPlayerController::OnSetDestinationReleased);
	}
	else
	{
		UE_LOG(LogBaseEntity, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ANPlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void ANPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
		
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ANPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}