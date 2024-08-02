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
#include "NexusTriumphant/NexusTriumphant.h"


ANPlayerController::ANPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bIsEnqueuing = false;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	InputDefinition = CreateDefaultSubobject<UNPlayerInputDef>(TEXT("Input Definition"));
	PlayerActionComponent = CreateDefaultSubobject<UNPlayerActionComponent>(TEXT("Player Action Component"));
}

void ANPlayerController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}


void ANPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
}

void ANPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	NPlayerState = GetPlayerState<ANPlayerState>();
	NPlayerCharacter = Cast<ANPlayerCharacter>(P);
	if (NPlayerCharacter && NPlayerState)
	{
		NPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(NPlayerState, NPlayerCharacter);
		NPlayerCharacter->SetPlayerState(NPlayerState);
		PlayerActionComponent->Setup(NPlayerState);
	}else
	{
		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Failed to cast player pawn (or state invalid)"));
	}
	//...
}

void ANPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	EILPSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(!IsValid(EnhancedInputComponent))
	{
		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Enhanced input component not found (failed cast)."));
		return;
	}
	if (IsValid(EILPSubsystem))
	{
		EILPSubsystem->ClearAllMappings();
		bool bExistInputs = false;
		if(InputDefinition)
		{
			// Add Input Mapping Context
			EILPSubsystem->AddMappingContext(InputDefinition->MappingContext, 1);
			if(InputDefinition->BindingMap.Num() < 1)
			{
				UE_LOG(LogNexusTriumphant, Warning, TEXT("[NPlayerController] Binding map is empty"));
			}
			for (auto Binding : InputDefinition->BindingMap)
			{
				UInputAction* InputAction = Binding.Value;
				TEnumAsByte<ENAbilityAction>& ActionEnum = Binding.Key;
				if(!IsValid(Binding.Value))
				{
					UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Invalid: Pair<%s , %d>"),
					ToCStr(InputAction->GetFullName()), int(ActionEnum));
				}
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ANPlayerController::OnInputStarted, ActionEnum);
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ANPlayerController::OnInputTriggered, ActionEnum);
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ANPlayerController::OnInputFinished, ActionEnum);
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &ANPlayerController::OnInputFinished, ActionEnum);
			}
			
			EnhancedInputComponent->BindAction(InputDefinition->EnqueueInput, ETriggerEvent::Started, this, &ANPlayerController::EnqueueStarted);
			EnhancedInputComponent->BindAction(InputDefinition->EnqueueInput, ETriggerEvent::Completed, this, &ANPlayerController::EnqueueEnded);
			EnhancedInputComponent->BindAction(InputDefinition->EnqueueInput, ETriggerEvent::Canceled, this, &ANPlayerController::EnqueueEnded);
			bExistInputs = true;
			UE_LOG(LogActionSystem, Display, TEXT("[Check Passed] InputDefinition Binding Map Bound"));
		}
		if(DefaultMappingContext)
		{ 
			EILPSubsystem->AddMappingContext(DefaultMappingContext, 0);
			// Setup mouse input events
			EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Started, this, &ANPlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Triggered, this, &ANPlayerController::OnSetDestinationTriggered);
			EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Completed, this, &ANPlayerController::OnSetDestinationReleased);
			EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Canceled, this, &ANPlayerController::OnSetDestinationReleased);
			bExistInputs = true;
		}
		if(!bExistInputs)
		{
			UE_LOG(LogNexusTriumphant, Warning, TEXT("[NPlayerController] No input mappings found"));
		}
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

void ANPlayerController::EnqueueStarted()
{
	bIsEnqueuing = true;
}

void ANPlayerController::EnqueueEnded()
{
	bIsEnqueuing = false;
}

void ANPlayerController::OnInputStarted(const TEnumAsByte<ENAbilityAction> InputUsed)
{
	UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Used AbilityAction #%d"), int(InputUsed));
	if(bIsEnqueuing)
	{
		PlayerActionComponent->EnqueueAction(InputUsed);
	}else
	{
		PlayerActionComponent->ExecuteAction(InputUsed);
	}
	
}

void ANPlayerController::OnInputTriggered(TEnumAsByte<ENAbilityAction> InputUsed)
{
}

void ANPlayerController::OnInputFinished(TEnumAsByte<ENAbilityAction> InputUsed)
{
	UE_LOG(LogActionSystem, Display, TEXT("Stopped using AbilityAction #%d"), int(InputUsed));
}

