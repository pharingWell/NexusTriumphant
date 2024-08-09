// Copyright Epic Games, Inc. All Rights Reserved.

#include "Entities/Player/NPlayerController.h"

#include "Entities/Player/NPlayerActionComponent.h"
#include "Entities/Player/NPlayerCharacter.h"
#include "NexusTriumphant/NexusTriumphant.h"


ANPlayerController::ANPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bIsEnqueuing = false;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedMoveToDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	InputDefinition = CreateDefaultSubobject<UNPlayerInputDef>(TEXT("Input Definition"));
	UNPlayerActionComponent* PlayerActionComponent;
	PlayerActionComponent = CreateDefaultSubobject<UNPlayerActionComponent>(TEXT("Player Action Component"));

}

UAbilitySystemComponent* ANPlayerController::GetAbilitySystemComponent() const
{
		
	if(NPS.Valid())
	{
		return NPS.ASC;
	}else if(IsValid(NPlayerState))
	{
		return NPlayerState->GetAbilitySystemComponent();
	}
	return nullptr;
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
	NPlayerCharacter = Cast<ANPlayerCharacter>(P);
	if (!IsValid(NPlayerCharacter))
	{
		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Failed to cast player pawn"));
		return;
	}
	CompleteSetup();
}

void ANPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	NPlayerState = GetPlayerState<ANPlayerState>();
	if (!IsValid(NPlayerState))
	{
		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Player state invalid)"));
		return;
	}
	CompleteSetup();
}

void ANPlayerController::CompleteSetup()
{
	if (!IsValid(NPlayerCharacter) || !IsValid(NPlayerState))
	{
		return;
	}
	if(!IsValid(NPlayerState->GetAbilitySystemComponent()))
	{
		UE_LOG(LogPlayerSystem, Warning, TEXT("[NPlayerController] Invalid Ability System Component"));
	}
	NPlayerCharacter->SetPlayerState(NPlayerState);
	NPS.Setup(NPActionComponent, NPlayerState, this, NPlayerCharacter, NPlayerState->GetAbilitySystemComponent());
}

void ANPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	EILPSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(!IsValid(EnhancedInputComponent.Get()))
	{
		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Enhanced input component not found (failed cast)."));
		return;
	}
	if (IsValid(EILPSubsystem))
	{
		EILPSubsystem->ClearAllMappings();
		bool bExistInputs = false;
		if(DefaultMappingContext)
		{ 
			EILPSubsystem->AddMappingContext(DefaultMappingContext, 0);
			// Setup mouse input events
			EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Started, this, &ANPlayerController::OnInputStarted, TEnumAsByte(MOVETO));
			EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Triggered, this, &ANPlayerController::OnInputTriggered, TEnumAsByte(MOVETO));
			EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Completed, this, &ANPlayerController::OnInputFinished, TEnumAsByte(MOVETO));
			EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Canceled, this, &ANPlayerController::OnInputFinished, TEnumAsByte(MOVETO));
			bExistInputs = true;
		}
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
				TEnumAsByte ActionEnum = Binding.Key;
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
		}else
		{
			UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Input Definition is invalid."))
		}
		
		if(!bExistInputs)
		{
			UE_LOG(LogNexusTriumphant, Warning, TEXT("[NPlayerController] No input mappings found"));
		}
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

void ANPlayerController::OnInputStarted(const TEnumAsByte<ENAbilityAction> InputUsed)
{
	UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Started AbilityAction #%d"), int(InputUsed));
	switch (InputUsed)
	{
		case MOVETO:
			StopMovement();
			break;
		default:
			if(bIsEnqueuing)
			{
				NPS.NPlayerActionComponent->EnqueueAction(InputUsed);
			}else
			{
				NPS.NPlayerActionComponent->ExecuteAction(InputUsed);
			}
			break;
	}
}

void ANPlayerController::OnInputTriggered(const TEnumAsByte<ENAbilityAction> InputUsed)
{
	UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Triggered AbilityAction #%d"), int(InputUsed));
	FHitResult Hit;
	APawn* ControlledPawn;
	switch (InputUsed)
	{
		case MOVETO:
			// We flag that the input is being pressed
				FollowTime += GetWorld()->GetDeltaSeconds();
		
			// We look for the location in the world where the player has pressed the input
			
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
				CachedMoveToDestination = Hit.Location;
			}
			
			// Move towards mouse pointer or touch
			ControlledPawn = GetPawn();
			if (ControlledPawn != nullptr)
			{
				FVector WorldDirection = (CachedMoveToDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
				ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
			}
			break;
		default:
			break;
	}
}

void ANPlayerController::OnInputFinished(const TEnumAsByte<ENAbilityAction> InputUsed)
{
	UE_LOG(LogActionSystem, Display, TEXT("Stopped using AbilityAction #%d"), int(InputUsed));
	switch (InputUsed)
	{
		case MOVETO:
			
			break;
		default:
			break;
	}
}

