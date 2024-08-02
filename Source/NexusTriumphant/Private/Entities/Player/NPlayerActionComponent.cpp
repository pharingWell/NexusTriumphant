

#include "Entities/Player/NPlayerActionComponent.h"

// Sets default values for this component's properties
UNPlayerActionComponent::UNPlayerActionComponent(const FObjectInitializer& ObjectInitializer) :
CurrentActionSpecHandle(BlankHandle), bExecutingQueue(false), NPlayerStateRef(nullptr),
ASCRef(nullptr), bASCRefValid(false), bSetup(false)

{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// This function connects the action component to the ability system component via the NPlayerState
void UNPlayerActionComponent::Setup(ANPlayerState* NPlayerState)
{
	if(IsValid(NPlayerState))
	{
		NPlayerStateRef = NPlayerState;
		ASCRef = NPlayerState->GetAbilitySystemComponent();
		bASCRefValid = IsValid(ASCRef);
		if(!bASCRefValid)
		{
			UE_LOG(LogActionSystem, Error, TEXT("[NPlayerActionComponent] Failed to get valid ASC ref"));
			return;
		}
		ASCRef->OnAbilityEnded.AddUFunction(this, "ActionEnded");
		bSetup = true;
		return;
	}
	UE_LOG(LogActionSystem, Error, TEXT("[NPlayerActionComponent] Failed to get valid PlayerState ref"));
	
}


/** Restores the Base Ability Action to the Current Ability Action slot */
void UNPlayerActionComponent::RevertAbilityAction(const ENAbilityAction Action)
{
	if(!bSetup) return;
	if(!CurrentAbilityActions.Contains(Action))
	{
		return;
	}
	if(!BaseAbilityActions.Contains(Action))
	{
		CurrentAbilityActions.Remove(Action);
		return;
	}
	CurrentAbilityActions[Action] = BaseAbilityActions[Action];
}



FGameplayAbilitySpecHandle& UNPlayerActionComponent::GetHandle(const ENAbilityAction Action, const bool GetBase)
{
	if(!bSetup)
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerActionComponent] GetHandle called before setup. Returning blank handle"))
		return BlankHandle;
	}
	if(GetBase)
	{
		if(BaseAbilityActions.Contains(Action))
		{
			checkf(!BaseAbilityActions[Action].IsValid(),
				TEXT("[NPlayerState] BaseAA assumption of validity failed, %s is invalid"),
				*BaseAbilityActions[Action].ToString()
			);
			return BaseAbilityActions[Action];
		}
		return BlankHandle;
	}

	if(CurrentAbilityActions.Contains(Action))
	{
		checkf(!CurrentAbilityActions[Action].IsValid(),
				TEXT("[NPlayerState] CurrentAA assumption of validity failed, %s is invalid"),
				*CurrentAbilityActions[Action].ToString()
			);
		return CurrentAbilityActions[Action];
	}
	
	return BlankHandle;
}


void UNPlayerActionComponent::ExecuteAction(const ENAbilityAction Action)
{
	if(!bSetup || !IsValid(NPlayerStateRef))
		return;
	ClearQueue();
	RunAbilityAction(Action); //discarding return
}

bool UNPlayerActionComponent::RunAbilityAction(const ENAbilityAction Action)
{
	if(!bSetup || !IsValid(NPlayerStateRef))
		return false;
	if(!bASCRefValid)
	{
		UpdateASCRef();
		if(!bASCRefValid)
			return false;
	}
	return ASCRef->TryActivateAbility(GetHandle(Action));
}

void UNPlayerActionComponent::CancelCurrentAction()
{
	if(!bSetup || !IsValid(NPlayerStateRef))
		return;
	ClearQueue();
	if(CurrentActionSpecHandle.IsValid())
	{
		ASCRef->CancelAbilityHandle(CurrentActionSpecHandle);
	}
	// TODO: Add better check for cancellation here
}

void UNPlayerActionComponent::EnqueueAction(const ENAbilityAction Action)
{
	if(!bSetup || !IsValid(NPlayerStateRef))
		return;
	Queue.Enqueue(Action);
	if(!bExecutingQueue)
	{
		ExecuteQueue();
	}
}

void UNPlayerActionComponent::ClearQueue()
{
	if(!bSetup || !IsValid(NPlayerStateRef))
		return;
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

void UNPlayerActionComponent::ExecuteQueue()
{
	if(!bSetup || !IsValid(NPlayerStateRef) || Queue.IsEmpty() || bExecutingQueue)
	{
		return;
	}
	bExecutingQueue = true;
	ExecuteQueuedAction();
}

/** Attempts to execute the next action in the queue, clearing the queue if an unexpected outcome is reached */
void UNPlayerActionComponent::ExecuteQueuedAction()
{
	if(!bSetup || !IsValid(NPlayerStateRef))
		return;
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
	FGameplayAbilitySpecHandle TempHandle = GetHandle(DequeuedAction);
	bool AbilityActivated = RunAbilityAction(DequeuedAction);
	if(!AbilityActivated)
	{
		// TODO: determine permutations when this is the case
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Ability Action #%d of Enum failed to run"),
			int(DequeuedAction));
		ClearQueue();
		return;
	}
	CurrentActionSpecHandle = TempHandle;
}

void UNPlayerActionComponent::ActionEnded(const FAbilityEndedData& AbilityEndedData)
{
	
	if(!bSetup || !IsValid(NPlayerStateRef))
		// should be unreachable, as action ended is bound during setup, but could be reached if the ref becomes invalid
		return;
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

