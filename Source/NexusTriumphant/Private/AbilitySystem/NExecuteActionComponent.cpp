// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem//NExecuteActionComponent.h"

#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/Abilities/NAbilityHelpers.h"
#include "Entities/Player/NPlayerController.h"
#include "NexusTriumphant/NexusTriumphant.h"

UNExecuteActionComponent::UNExecuteActionComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bComponentInitialized = false;
}

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
}

void UNExecuteActionComponent::ExecuteAction(const FGameplayAbilitySpecHandle& Action)
{
	if(!bComponentInitialized) return;
	ClearQueue();
	bool AbilityActivated = GetAbilitySystemComponent()->TryActivateAbility(Action, true);
}

void UNExecuteActionComponent::CancelCurrentAction()
{
	if(!bComponentInitialized) return;
	ClearQueue();
	if(CurrentActionSpecHandle.IsValid())
	{
		GetAbilitySystemComponent()->CancelAbilityHandle(CurrentActionSpecHandle);
	}
}

void UNExecuteActionComponent::EnqueueAction(const FGameplayAbilitySpecHandle& Action)
{
	if(!bComponentInitialized) return;
	Queue.Enqueue(Action);
	if(!bExecutingQueue)
	{
		ExecuteQueue();
	}
}

void UNExecuteActionComponent::ClearQueue()
{
	if(!bComponentInitialized) return;
	Queue.Empty();
	bExecutingQueue = false;
}

/*
 * Execute Queue
 * The queue is executed FIFO, with the first ability added being executed first
 * After an ability ends (via EndAbility) the queue continues with the next ability
 * If any non-queued player input is given, or an ability is cancelled, the queue is cleared:
 * Using an action without holding the "Queue Action" key bind, or an ability returns cancelled, the queue clears
 */

void UNExecuteActionComponent::ExecuteQueue()
{
	if(!bComponentInitialized) return;
	if(Queue.IsEmpty() || bExecutingQueue)
	{
		return;
	}
	bExecutingQueue = true;
	ExecuteQueuedAction();
}

void UNExecuteActionComponent::ExecuteQueuedAction()
{
	if(!bComponentInitialized) return;
	if(Queue.IsEmpty() || !bExecutingQueue)
	{
		return;
	}
	FGameplayAbilitySpecHandle SpecHandle;
	Queue.Dequeue(SpecHandle);
	if(!SpecHandle.IsValid())
	{
		UE_LOG_ABILITY_CAUTION("[Action Queue] Invalid ability spec handle dequeued", this);
		ExecuteQueuedAction();
	}
	bool AbilityActivated = GetAbilitySystemComponent()->TryActivateAbility(SpecHandle, true);
	if(!AbilityActivated)
	{
		ClearQueue();
		return;
	}
	CurrentActionSpecHandle = SpecHandle;
}

void UNExecuteActionComponent::ActionEnded(const FAbilityEndedData& AbilityEndedData)
{
	if(!bComponentInitialized) return;
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


