// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/NPlayerController.h"

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
#include "GameFramework/PawnMovementComponent.h"
#include "NexusTriumphant/NexusTriumphant.h"
#include "Player/NPlayerCharacter.h"


ANPlayerController::ANPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bIsEnqueuing = false;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedMoveToDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	PlayerActionComponent = CreateDefaultSubobject<UNPlayerActionComponent>(TEXT("Player Action Component"));
	NAbilitySystemComponent = CreateDefaultSubobject<UNAbilitySystemComponent>(TEXT("NAbilitySystemComponent"));
}

void ANPlayerController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ANPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}


void ANPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	PlayerActionComponent = NewObject<UNPlayerActionComponent>(this, UNPlayerActionComponent::StaticClass());
	PlayerActionComponent->RegisterComponent();
	NAbilitySystemComponent = NewObject<UNAbilitySystemComponent>(this, UNAbilitySystemComponent::StaticClass());
	NAbilitySystemComponent->RegisterComponent();
}

void ANPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ANPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ANPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	NPlayerState = GetPlayerState<ANPlayerState>();
	NPlayerCharacter = Cast<ANPlayerCharacter>(P);
	if (!IsValid(PlayerActionComponent))
	{
		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Player Action Component not ready"));
		return;
	}
	if (IsValid(NPlayerState))
	{
		if PlayerActionComponent-
		PlayerActionComponent->Setup(NPlayerState, this);
	}
	// {
	// 	NPlayerCharacter->SetPlayerState(NPlayerState);
	// 	if (NASC)
	// 	{
	// 		NASC->InitAbilityActorInfo(this, NPlayerCharacter);
	// 	} else
	// 	{
	// 		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] NASC not ready"));
	// 		return;
	// 	}
	// }else
	// {
	// 	UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Failed to cast player pawn (or state invalid)"));
	// }
	//...
}

void ANPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
}

void ANPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	 
	// When we're a client connected to a remote server, the player controller may replicate later than the PlayerState and AbilitySystemComponent.
	if (GetWorld()->IsNetMode(NM_Client))
	{
		if (!NAbilitySystemComponent)
			NAbilitySystemComponent = GetNAbilitySystemComponent();
		if (NAbilitySystemComponent)
			{
				// Calls InitAbilityActorInfo
				NAbilitySystemComponent->RefreshAbilityActorInfo();
				// NASC->TryActivateAbilitiesOnSpawn();
			}
		}
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
		if(IsValid(NMappingContext))
		{
			// Add Input Mapping Context
			EILPSubsystem->AddMappingContext(NMappingContext, 0);
			if(NMappingContext->GetMappings().Num() < 1)
			{
				UE_LOG(LogNexusTriumphant, Warning, TEXT("[NPlayerController] Binding map is empty"));
			}
			
			for (auto& Element : NMappingContext->GetNMappings())
			{
				ENAbilityAction ActionEnum = Element.Enum;
				const UInputAction* InputAction = Element.Action;
				if(ActionEnum == ENAbilityAction::ENQUEUE)
				{
					EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ANPlayerController::EnqueueStarted);
					EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ANPlayerController::EnqueueEnded);
					EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &ANPlayerController::EnqueueEnded);
					continue;
				}
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ANPlayerController::OnInputStarted, ActionEnum);
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ANPlayerController::OnInputTriggered, ActionEnum);
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ANPlayerController::OnInputFinished, ActionEnum);
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &ANPlayerController::OnInputFinished, ActionEnum);
			}
			
		}else
		{
			UE_LOG(LogActionSystem, Error, TEXT("[NPlayerController] Input Definition Mapping Context is invalid."))
		}
	}else
	{
		UE_LOG(LogActionSystem, Error, TEXT("[NPlayerController] Enhanced Input Local Player is invalid."))
	}
}

void ANPlayerController::EnqueueStarted()
{
	bIsEnqueuing = true;
}

void ANPlayerController::EnqueueEnded()
{
	bIsEnqueuing = false;
}

void ANPlayerController::OnInputStarted(const ENAbilityAction InputUsed)
{
	UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Started AbilityAction #%d"), int(InputUsed));
	if (InputUsed == ENAbilityAction::MOVETO)
	{
		StopMovement();
	}
	
}

void ANPlayerController::OnInputTriggered(const ENAbilityAction InputUsed)
{
	UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Triggered AbilityAction #%d"), int(InputUsed));
	
	FGameplayEventData EventData;
	PlayerActionComponent->ApplyInput(InputUsed, ENAbilityCastMode::INSTANT, bIsEnqueuing);
}

void ANPlayerController::OnInputFinished(const ENAbilityAction InputUsed)
{
	UE_LOG(LogActionSystem, Display, TEXT("Stopped using AbilityAction #%d"), int(InputUsed));

}



bool ANPlayerController::K2_GetHitResultUnderCursor(ECollisionChannel TraceChannel, bool bTraceComplex,
                                                    FHitResult& HitResult)
{
	return GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
}

