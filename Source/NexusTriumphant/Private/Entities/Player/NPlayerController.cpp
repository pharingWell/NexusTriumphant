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


ANPlayerController::ANPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;

	ExecuteActionComponent = CreateDefaultSubobject<UNExecuteActionComponent>(TEXT("ExecuteActionComponent"));
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

// Fill out your copyright notice in the Description page of Project Settings.



void ANPlayerController::ExecuteAction(const ENAbilityAction Action)
{
	if(!NPlayerState) return;
	ClearQueue();
	bool AbilityActivated = NPlayerState->RunAbilityAction(Action);
}

void ANPlayerController::CancelCurrentAction()
{
	ClearQueue();
	if(CurrentActionSpecHandle.IsValid())
	{
		GetAbilitySystemComponent()->CancelAbilityHandle(NPlayerState->GetHandle(CurrentAbilityAction));
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
	CurrentActionSpecHandle = BlankHandle;
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

void ANPlayerController::ExecuteQueuedAction()
{
	if(Queue.IsEmpty() || !bExecutingQueue)
	{
		return;
	}
	ENAbilityAction DequeuedAction;
	Queue.Dequeue(DequeuedAction);
	if(!DequeuedAction)
	{
		// TODO figure out what goes here
	}
	FGameplayAbilitySpecHandle TempHandle = NPlayerState->GetHandle(DequeuedAction);
	bool AbilityActivated = NPlayerState->RunAbilityAction(DequeuedAction);
	if(!AbilityActivated)
	{
		// TODO: determine permutations when this is the case
		//ClearQueue();
		return;
	}
	CurrentActionSpecHandle = TempHandle;
}

void ANPlayerController::ActionEnded(const FAbilityEndedData& AbilityEndedData)
{
	if(AbilityEndedData.AbilitySpecHandle == CurrentActionSpecHandle)
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
