

#include "Player/NPlayerActionComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "Player/NPlayerController.h"

struct FGameplayAbilityTargetDataHandle;

// Sets default values for this component's properties
UNPlayerActionComponent::UNPlayerActionComponent(const FObjectInitializer& ObjectInitializer) :
	bExecutingQueue(false), NPlayerController(nullptr), NPlayerState(nullptr),
	ASCRef(nullptr), bSetup(false), bPlay(false)

{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
	BlankHandle = FGameplayAbilitySpecHandle();
	CurrentActionSpecHandle = BlankHandle;
	// ...
}

void UNPlayerActionComponent::BeginPlay()
{
	Super::BeginPlay();
	bPlay = true;
}

// This function connects the action component to the ability system component via the NPlayerState
void UNPlayerActionComponent::Setup(const TObjectPtr<ANPlayerState>& InPlayerState, const TObjectPtr<ANPlayerController>& InPlayerController)
{
	if(bSetup)
	{
		return;
	}
	if(!IsValid(InPlayerController))
	{
		UE_LOG(LogActionSystem, Error, TEXT("[NPlayerActionComponent] Failed to get valid PlayerController ref"));
		return;
	}
	NPlayerController = InPlayerController;
	if(!IsValid(InPlayerState))
	{
		UE_LOG(LogActionSystem, Error, TEXT("[NPlayerActionComponent] Failed to get valid PlayerState ref"));
		return;
	}
	NPlayerState = InPlayerState;
	ASCRef = InPlayerController->GetAbilitySystemComponent();
	if(!IsValid(ASCRef))
	{
		UE_LOG(LogActionSystem, Error, TEXT("[NPlayerActionComponent] Failed to get valid ASC ref"));
		return;
	}
	if (!IsValid(NPlayerState->GetChampionDataAsset()))
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerActionComponent] Invalid Champion Def"));
		return;
	}
	if(bPlay)
	{
		UE_LOG(LogActionSystem, Warning, TEXT("Setup correctly"));
		return;
	}

	ASCRef->OnAbilityEnded.AddUFunction(this, "ActionEnded");
	TMap<ENAbilityAction, FString> Names {};
	if(NPlayerController->HasAuthority())
	{
		for (const auto AbilityPair : NPlayerState->GetChampionDataAsset()->GetUpdatedAbilityMap())
		{
			const ENAbilityAction &AbilityAction = AbilityPair.Key;
			if(AbilityAction == ENAbilityAction::ENQUEUE)
			{
				continue;
			}
			const TSubclassOf<UGameplayAbility> &GameplayAbility = AbilityPair.Value;
			
			BaseAbilityActions.Add(AbilityAction, ASCRef->GiveAbility(
			FGameplayAbilitySpec(GameplayAbility, 1 /* abil level */, static_cast<uint8>(AbilityAction), this)));
			Names.Add(AbilityAction, GameplayAbility->GetDescription());
		}

		CurrentAbilityActions = BaseAbilityActions;
	
		FString String = "";
		for (auto Element : BaseAbilityActions)
		{
			String += FString::Printf(TEXT("[%d, %s, %s]"), Element.Key, *Element.Value.ToString(), *Names[Element.Key]);
		}
		UE_LOG(LogActionSystem, Display, TEXT("[NPlayerActionComponent] CurrentAbilityActions: {%s}"), *String);
	}
	bSetup = true;

}

UAbilitySystemComponent* UNPlayerActionComponent::GetAbilitySystemComponent() const
{
	if(IsValid(ASCRef))
	{
		return ASCRef;
	}
	if(!IsValid(NPlayerController))
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerActionComponent] GetASC before setup/while NPlayerController ref invalid"));
		return nullptr;
	}
	return NPlayerController->GetAbilitySystemComponent();
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



FGameplayAbilitySpecHandle& UNPlayerActionComponent::GetHandle(const ENAbilityAction Action, const bool GetBase /* false */)
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
			checkf(BaseAbilityActions[Action].IsValid(),
				TEXT("[NPlayerState] BaseAbilityActions assumption of validity failed, %s is invalid"),
				*BaseAbilityActions[Action].ToString()
			);
			return BaseAbilityActions[Action];
		}
		return BlankHandle;
	}

	if(CurrentAbilityActions.Contains(Action))
	{
		checkf(CurrentAbilityActions[Action].IsValid(),
				TEXT("[NPlayerState] CurrentAbilityActions assumption of validity failed, %s is invalid"),
				*CurrentAbilityActions[Action].ToString()
			);
		return CurrentAbilityActions[Action];
	}
	
	return BlankHandle;
}

void UNPlayerActionComponent::ApplyInput(const ENAbilityAction InputUsed, const  ENAbilityCastMode CastMode, bool bIsEnqueueing) {
	FGameplayEventData EventData;
	bool bValidTarget = false;
	FHitResult Hit;
	switch (InputUsed)
	{
		case ENAbilityAction::MOVETO:
						// Move towards mouse pointer or touch
			if(NPlayerController)
			{
				bValidTarget = NPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
			} else
			{
				UE_LOG(LogActionSystem, Warning, TEXT("[%s] ApplyInput detected an invalid NPlayerController (%s)"), *this->GetReadableName(), *GetOuter()->GetFullName())
		
			}
			EventData.TargetData.Append(MakeTargetDataHandleFromHitResult(Hit));
			
			// // If we hit a surface, cache the location
			// if (bHitSuccessful)
			// {
			// 	CachedMoveToDestination = Hit.Location;
			// }
			break;
		default:
			break;
	}
	if(bValidTarget)
		if(bIsEnqueueing)
		{
			UE_LOG(LogActionSystem, Warning, TEXT("Queue is on"));
			EnqueueAction(InputUsed, EventData);
		}else
		{
			// if(bShouldClearQueue)
			ClearQueue();
			ExecuteAction(InputUsed, EventData);
		}
}


bool UNPlayerActionComponent::ExecuteAction(const ENAbilityAction Action, const FGameplayEventData& EventData)
{
	if(!bSetup || !IsValid(NPlayerController))
		return false;
	NPlayerController->Server_RunAbilityAction(Action, EventData);
	return true;
}

void UNPlayerActionComponent::CancelCurrentAction()
{
	if(!bSetup || !IsValid(NPlayerState))
		return;
	ClearQueue();
	if(CurrentActionSpecHandle.IsValid())
	{
		ASCRef->CancelAbilityHandle(CurrentActionSpecHandle);
	}
	// TODO: Add better check for cancellation here
}

void UNPlayerActionComponent::EnqueueAction(const ENAbilityAction Action, const FGameplayEventData& EventData)
{
	if(!bSetup || !IsValid(NPlayerState))
		return;
	Queue.Enqueue(FNActionQueueEntry(Action, EventData));
	if(!bExecutingQueue)
	{
		ExecuteQueue();
	}
}

void UNPlayerActionComponent::ClearQueue()
{
	if(!bSetup || !IsValid(NPlayerState))
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
	if(!bSetup || !IsValid(NPlayerState) || Queue.IsEmpty() || bExecutingQueue)
	{
		return;
	}
	bExecutingQueue = true;
	ExecuteQueuedAction();
}

/** Attempts to execute the next action in the queue, clearing the queue if an unexpected outcome is reached */
void UNPlayerActionComponent::ExecuteQueuedAction()
{
	if(!bSetup || !IsValid(NPlayerState))
		return;
	if(Queue.IsEmpty() || !bExecutingQueue)
	{
		ClearQueue();
		return;
	}

	FNActionQueueEntry DequeuedAction = FNActionQueueEntry(ENAbilityAction::INVALID);
	Queue.Dequeue(DequeuedAction);
	if(DequeuedAction.AbilityAction == ENAbilityAction::INVALID)
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Encountered invalid action in queue"))
		ExecuteQueuedAction(); // causes recursion, if this breaks the stack you have worse problems
		return;
	}
	FGameplayAbilitySpecHandle TempHandle = GetHandle(DequeuedAction.AbilityAction); // I'm decently sure this causes undefined behavior
	bool AbilityActivated = ExecuteAction(DequeuedAction.AbilityAction, DequeuedAction.EventData);
	if(!AbilityActivated)
	{
		// TODO: determine permutations when this is the case
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerActionComponent] Ability Action #%d of Enum failed to run"),
			DequeuedAction.AbilityAction);
		ClearQueue();
		return;
	}
	CurrentActionSpecHandle = TempHandle;
}

void UNPlayerActionComponent::ActionEnded(const FAbilityEndedData& AbilityEndedData)
{
	
	if(!bSetup || !IsValid(NPlayerState))
		// should be unreachable, as action ended is bound during setup, but could be reached if the ref becomes invalid
		return;
	if(CurrentActionSpecHandle.IsValid() &&
		AbilityEndedData.AbilitySpecHandle == CurrentActionSpecHandle)
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


FGameplayAbilityTargetDataHandle UNPlayerActionComponent::MakeTargetDataHandleFromHitResult(const FHitResult& HitResult)
{
	FGameplayAbilityTargetDataHandle ReturnDataHandle;

	/** Note: These are cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
	FGameplayAbilityTargetData_SingleTargetHit* ReturnData = new FGameplayAbilityTargetData_SingleTargetHit();
	ReturnData->HitResult = HitResult;
	ReturnDataHandle.Add(ReturnData);
	return ReturnDataHandle;
}

FGameplayAbilityTargetDataHandle UNPlayerActionComponent::MakeTargetDataHandleFromHitResults(const TArray<FHitResult>& HitResults)
{
	FGameplayAbilityTargetDataHandle ReturnDataHandle;

	for (int32 i = 0; i < HitResults.Num(); i++)
	{
		/** Note: These are cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
		FGameplayAbilityTargetData_SingleTargetHit* ReturnData = new FGameplayAbilityTargetData_SingleTargetHit();
		ReturnData->HitResult = HitResults[i];
		ReturnDataHandle.Add(ReturnData);
	}
	
	return ReturnDataHandle;
}