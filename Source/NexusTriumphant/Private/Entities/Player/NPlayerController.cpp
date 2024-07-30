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
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	InputDefinition = CreateDefaultSubobject<UNPlayerInputDef>(TEXT("Input Definition"));
}

void ANPlayerController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	NPlayerState = StaticCast<ANPlayerState*>(GetPlayerState<ANPlayerState>());
	if(!NPlayerState)
	{
		
	}
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
	}else
	{
		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Failed to cast player state or pawn"));
	}
	UpdateASCRef();
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
			for (auto Binding : InputDefinition->BindingMap)
			{
				EnhancedInputComponent->BindAction(Binding.Value, ETriggerEvent::Started, this, &ANPlayerController::OnInputStarted, Binding.Key);
				EnhancedInputComponent->BindAction(Binding.Value, ETriggerEvent::Triggered, this, &ANPlayerController::OnInputTriggered, Binding.Key);
				EnhancedInputComponent->BindAction(Binding.Value, ETriggerEvent::Completed, this, &ANPlayerController::OnInputFinished, Binding.Key);
				EnhancedInputComponent->BindAction(Binding.Value, ETriggerEvent::Canceled, this, &ANPlayerController::OnInputFinished, Binding.Key);
			}
			bExistInputs = true;
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

bool ANPlayerController::RunAbilityAction(ENAbilityAction Action)
{
	if(!NPlayerState)
		return false;
	if(!bASCRefValid)
	{
		UpdateASCRef();
		if(!bASCRefValid)
			return false;
	}
	return NPlayerState->RunAction(Action);
}

void ANPlayerController::ExecuteAction(const ENAbilityAction Action)
{
	if(!NPlayerState) return;
	ClearQueue();
	bool AbilityActivated = RunAbilityAction(Action);
}

void ANPlayerController::CancelCurrentAction()
{
	ClearQueue();
	if(CurrentAction != INVALID)
	{
		//NPlayerState->CancelAction(); ???
	}
	// TODO: Add better check for cancellation here
}

void ANPlayerController::EnqueueAction(const ENAbilityAction Action)
{
	if(!NPlayerState) return;
	Queue.Enqueue(Action);
	if(!bExecutingQueue)
	{
		ExecuteQueue();
	}
}

void ANPlayerController::ClearQueue()
{
	Queue.Empty();
	bExecutingQueue = false;
	CurrentAction = INVALID;
}

/*
 * Execute Queue
 * The queue is executed FIFO, with the first ability added being executed first
 * After an ability ends (via EndAbility) the queue continues with the next ability
 * If any non-queued player input is given, or an ability is cancelled, the queue is cleared:
 * Using an action without holding the "Queue Action" key bind, or an ability returns cancelled, the queue clears
 */

void ANPlayerController::ExecuteQueue()
{
	if(!NPlayerState || Queue.IsEmpty() || bExecutingQueue)
	{
		return;
	}
	bExecutingQueue = true;
	ExecuteQueuedAction();
}

/** Attempts to execute the next action in the queue, clearing the queue if an unexpected outcome is reached */
void ANPlayerController::ExecuteQueuedAction()
{
	if(Queue.IsEmpty() || !bExecutingQueue)
	{
		ClearQueue();
		return;
	}

	ENAbilityAction DequeuedAction = INVALID;
	Queue.Dequeue(DequeuedAction);
	if(DequeuedAction == INVALID)
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Encountered invalid action in queue"))
		ExecuteQueuedAction(); // causes recursion, if this breaks the stack you have worse problems
		return;
	}
	//FGameplayAbilitySpecHandle TempHandle = NPlayerState->GetHandle(DequeuedAction);
	bool AbilityActivated = RunAbilityAction(DequeuedAction);
	if(!AbilityActivated)
	{
		// TODO: determine permutations when this is the case
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Ability Action #%d of Enum failed to run"),
			int(DequeuedAction));
		ClearQueue();
		return;
	}
	CurrentAction = DequeuedAction;
}

/*void ANPlayerController::ActionEnded(const FAbilityEndedData& AbilityEndedData)
{
	if(AbilityEndedData.AbilitySpecHandle == )
	{
		if(bExecutingQueue){
			if(AbilityEndedData.bWasCancelled)
			{
				ClearQueue();
				return;
			}
			if(Queue.IsEmpty())
			{
				bExecutingQueue = false;
				return;
			}
			ExecuteQueuedAction();
		}
	}
}*/

void ANPlayerController::OnInputStarted(TEnumAsByte<ENAbilityAction> InputUsed)
{
	UE_LOG(LogActionSystem, Display, TEXT("Used AbilityAction #%d"), int(InputUsed));
	
}

void ANPlayerController::OnInputTriggered(TEnumAsByte<ENAbilityAction> InputUsed)
{
}

void ANPlayerController::OnInputFinished(TEnumAsByte<ENAbilityAction> InputUsed)
{
	UE_LOG(LogActionSystem, Display, TEXT("Stopped using AbilityAction #%d"), int(InputUsed));
}


/*
void UNExecuteActionComponent::InitializeComponent()
{
	
	
	GetAbilitySystemComponent()->OnAbilityEnded.AddUFunction(this, "ActionEnded");

	ANPlayerController* Controller = Cast<ANPlayerController>(GetOwner());
	if(Controller != nullptr)
	{
		ANPlayerCharacter* Actor = Cast<ANPlayerCharacter>(Controller->GetCharacter());
		if(Actor != nullptr)
		{
			AvatarActor = Actor;
			if(ANPlayerState* PlayerState = AvatarActor->GetPlayerState<ANPlayerState>())
			{
				OwningState = PlayerState;
				AbilityActorInfo.InitFromActor(OwningState, AvatarActor, UNExecuteActionComponent::GetAbilitySystemComponent());
			}else
			{
				UE_LOG(LogNexusTriumphant, Warning, TEXT("[NExecuteActionComponent] Incorrect type of player state. Destroying"));
				UActorComponent::DestroyComponent();
				return;
			}
			bComponentInitialized = true;
		}else
		{
			UE_LOG(LogNexusTriumphant, Warning, TEXT("[NExecuteActionComponent] Incorrect type of owning actor. Destroying"));
			UActorComponent::DestroyComponent();
			return;
		}
	}
}*/
