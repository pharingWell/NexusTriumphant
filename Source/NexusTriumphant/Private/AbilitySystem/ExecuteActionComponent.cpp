// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem//ExecuteActionComponent.h"

#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/Abilities/NAbilityHelpers.h"

UExecuteActionComponent::UExecuteActionComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	ANPlayerCharacter* Actor = Cast<ANPlayerCharacter>(GetOwner());
	if(Actor != nullptr)
	{
		AvatarActor = Actor;
		if(ANPlayerState* PlayerState = AvatarActor->GetPlayerState<ANPlayerState>())
		{
			OwningState = PlayerState;
			AbilityActorInfo.InitFromActor(OwningState, AvatarActor, UExecuteActionComponent::GetAbilitySystemComponent());
		}
	}else
	{
		UE_LOG_ABILITY_CAUTION("Fuck", this);
		//USceneComponent::DestroyComponent();
	}
	
}

void UExecuteActionComponent::ExecuteAction(FGameplayAbilitySpecHandle& Action)
{
	ClearQueue();
}

void UExecuteActionComponent::CancelCurrentAction()
{
	ClearQueue();
	if(CurrentAction.IsValid())
	{
		GetAbilitySystemComponent()->CancelAbilityHandle(CurrentAction);
	}
}

void UExecuteActionComponent::EnqueueAction(const FGameplayAbilitySpecHandle& Action)
{
	Queue.Enqueue(Action);
	if(!bExecutingQueue)
	{
		ExecuteQueue();
	}
}

void UExecuteActionComponent::ClearQueue()
{
}

/*
 * Execute Queue
 * The queue is executed FIFO, with the first ability added being executed first
 * After an ability ends (via EndAbility) the queue continues with the next ability
 * If any non-queued player input is given, or an ability is cancelled, the queue is cleared:
 * Using an action without holding the "Queue Action" key bind, or an ability returns cancelled, the queue clears
 */

void UExecuteActionComponent::ExecuteQueue()
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
