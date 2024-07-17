// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem//NExecuteActionComponent.h"

#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/Abilities/NAbilityHelpers.h"

UNExecuteActionComponent::UNExecuteActionComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	
}

void UNExecuteActionComponent::InitializeComponent()
{
	
	ANPlayerCharacter* Actor = Cast<ANPlayerCharacter>(GetOwner());
	if(Actor != nullptr)
	{
		AvatarActor = Actor;
		if(ANPlayerState* PlayerState = AvatarActor->GetPlayerState<ANPlayerState>())
		{
			OwningState = PlayerState;
			AbilityActorInfo.InitFromActor(OwningState, AvatarActor, UNExecuteActionComponent::GetAbilitySystemComponent());
		}
	}else
	{
		UE_LOG_ABILITY_CAUTION("Fuck", this);
		UActorComponent::DestroyComponent();
	}
}

void UNExecuteActionComponent::ExecuteAction(FGameplayAbilitySpecHandle& Action)
{
	ClearQueue();
}

void UNExecuteActionComponent::CancelCurrentAction()
{
	ClearQueue();
	if(CurrentAction.IsValid())
	{
		GetAbilitySystemComponent()->CancelAbilityHandle(CurrentAction);
	}
}

void UNExecuteActionComponent::EnqueueAction(const FGameplayAbilitySpecHandle& Action)
{
	Queue.Enqueue(Action);
	if(!bExecutingQueue)
	{
		ExecuteQueue();
	}
}

void UNExecuteActionComponent::ClearQueue()
{
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
	if(Queue.IsEmpty() || bExecutingQueue)
	{
		return;
	}
	bExecutingQueue = true;
	while(!Queue.IsEmpty())
	{
		FGameplayAbilitySpecHandle SpecHandle;
		Queue.Dequeue(SpecHandle);
		if(!SpecHandle.IsValid())
		{
			UE_LOG_ABILITY_CAUTION("[Action Queue] Invalid ability spec handle dequeued", this);
			continue;
		}
		bool AbilityActivated = GetAbilitySystemComponent()->TryActivateAbility(SpecHandle, true);
		if(!AbilityActivated)
		{
			ClearQueue();
			return;
		}
		CurrentAction = SpecHandle;
		
	}
	bExecutingQueue = false;
}
