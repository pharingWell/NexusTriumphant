#include "Entities/Player/NPlayerActionComponent.h"

#include "GameplayTags.h"
#include "GameplayTagContainer.h"
#include "Entities/Player/NPlayerController.h"
#include "Entities/Player/NPlayerState.h"


// Sets default values for this component's properties
UNPlayerActionComponent::UNPlayerActionComponent(const FObjectInitializer& ObjectInitializer) :
	CurrentActionSpecHandle(nullptr), bExecutingQueue(false), NPS(nullptr), bSetup(false), bPlay(false)
// NPlayerController(nullptr), NPlayerStateRef(nullptr), ASCRef(nullptr), bASCRefValid(false),
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
	BlankHandle = FGameplayAbilitySpecHandle();
	// ...
}

void UNPlayerActionComponent::BeginPlay()
{
	Super::BeginPlay();
	bPlay = true;
}

/** Restores the Base Ability Action to the Current Ability Action slot */
void UNPlayerActionComponent::RevertAbilityAction(const ENAbilityAction Action)
{
	if (!bSetup) return;
	if (!CurrentAbilityActions.Contains(Action))
	{
		return;
	}
	if (!BaseAbilityActions.Contains(Action))
	{
		CurrentAbilityActions.Remove(Action);
		return;
	}
	CurrentAbilityActions[Action] = BaseAbilityActions[Action];
}

FGameplayAbilitySpecHandle& UNPlayerActionComponent::GetHandle(const ENAbilityAction Action, const bool GetBase)
{
	if (!bSetup)
	{
		UE_LOG(LogActionSystem, Warning,
		       TEXT("[NPlayerActionComponent] GetHandle called before setup. Returning blank handle"))
		return BlankHandle;
	}
	if (GetBase)
	{
		if (BaseAbilityActions.Contains(Action))
		{
			checkf(!BaseAbilityActions[Action].IsValid(),
			       TEXT("[NPlayerState] BaseAA assumption of validity failed, %s is invalid"),
			       *BaseAbilityActions[Action].ToString()
			);
			return BaseAbilityActions[Action];
		}
		return BlankHandle;
	}

	if (CurrentAbilityActions.Contains(Action))
	{
		checkf(!CurrentAbilityActions[Action].IsValid(),
		       TEXT("[NPlayerState] CurrentAA assumption of validity failed, %s is invalid"),
		       *CurrentAbilityActions[Action].ToString()
		);
		return CurrentAbilityActions[Action];
	}

	return BlankHandle;
}
// Single function that triggers Gameplay Ability
bool UNPlayerActionComponent::RunAbilityAction(const ENAbilityAction Action)
{
	if (NPS->Valid())
		return false;
	if (NPS->NPlayerState->HasAuthority())
	{
		FGameplayEventData EventData;
		EventData.OptionalObject = NPS->NPlayerController;
		FGameplayTag Tag =  FGameplayTag::EmptyTag; //::RequestGameplayTag(FName("Ability.Used"));
		return NPS->ASC->TriggerAbilityFromGameplayEvent(GetHandle(Action), NPS->AbilityActorInfo.Get(),
			Tag, &EventData, *NPS->ASC);
	}
	UE_LOG(LogActionSystem, Warning,
	       TEXT("[NPlayerActionComponent] Failed to activate ability because lacking authority"));
	return false;
}

void UNPlayerActionComponent::CancelCurrentAction()
{
	if (!NPS->Valid())
		return;
	ClearQueue();
	if (CurrentActionSpecHandle->IsValid())
	{
		NPS->ASC->CancelAbilityHandle(*CurrentActionSpecHandle);
	}
	// TODO: Add better check for cancellation here
}

void UNPlayerActionComponent::ExecuteAction(const ENAbilityAction Action)
{
	if (!NPS->Valid())
		return;
	ClearQueue();
	bool bDidAbilityRun = RunAbilityAction(Action);
	if (!bDidAbilityRun)
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerActionComponent] Ran Action #%d, but it failed to activate"),
		       Action);
	}
}

void UNPlayerActionComponent::EnqueueAction(const ENAbilityAction Action)
{
	if (!NPS->Valid())
		return;
	Queue.Enqueue(Action);
	if (!bExecutingQueue)
	{
		ExecuteQueue();
	}
}

void UNPlayerActionComponent::ClearQueue()
{
	if (!NPS->Valid())
		return;
	Queue.Empty();
	bExecutingQueue = false;
	CurrentActionSpecHandle = nullptr;
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
	if (!bSetup || !NPS->Valid() || Queue.IsEmpty() || bExecutingQueue)
	{
		return;
	}
	bExecutingQueue = true;
	ExecuteQueuedAction();
}

/** Attempts to execute the next action in the queue, clearing the queue if an unexpected outcome is reached */
void UNPlayerActionComponent::ExecuteQueuedAction()
{
	if (!bSetup || !NPS->Valid())
		return;
	if (Queue.IsEmpty() || !bExecutingQueue)
	{
		ClearQueue();
		return;
	}

	ENAbilityAction DequeuedAction = INVALID;
	Queue.Dequeue(DequeuedAction);
	if (DequeuedAction == INVALID)
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Encountered invalid action in queue"))
		ExecuteQueuedAction(); // causes recursion, if this breaks the stack you have worse problems
		return;
	}
	FGameplayAbilitySpecHandle* TempHandlePtr = &GetHandle(DequeuedAction);
	// I'm decently sure this causes undefined behavior
	bool AbilityActivated = RunAbilityAction(DequeuedAction);
	if (!AbilityActivated)
	{
		// TODO: determine permutations when this is the case
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Ability Action #%d of Enum failed to run"),
		       int(DequeuedAction));
		ClearQueue();
		return;
	}
	CurrentActionSpecHandle = TempHandlePtr;
}

void UNPlayerActionComponent::ActionEnded(const FAbilityEndedData& AbilityEndedData)
{
	if (!bSetup || !NPS->Valid())
		// should be unreachable, as action ended is bound during setup, but could be reached if the ref becomes invalid
		return;
	if (AbilityEndedData.AbilitySpecHandle == *CurrentActionSpecHandle)
	{
		if (bExecutingQueue)
		{
			if (AbilityEndedData.bWasCancelled)
			{
				ClearQueue();
				return;
			}
			if (Queue.IsEmpty())
			{
				bExecutingQueue = false;
				return;
			}
			ExecuteQueuedAction();
		}
	}
}

void UNPlayerActionComponent::SetupNPS(FNPlayerSystem* InNPS)
{
	NPS = InNPS;

	if (bPlay)
	{
		UE_LOG(LogActionSystem, Warning, TEXT("Setup correctly"));
	}
	if (NPS->NPlayerState->HasAuthority())
	{
		NPS->ASC->OnAbilityEnded.AddUFunction(this, "ActionEnded");

		TMap<TEnumAsByte<ENAbilityAction>, FString> Names{};
		for (auto AbilityPair : NPS->NPlayerState->GetChampionDataAsset()->GetUpdatedAbilityMap())
		{
			if (IsValid(AbilityPair.Value))
			{
				Names.Add(AbilityPair.Key, AbilityPair.Value->GetDescription());
				BaseAbilityActions.Add(AbilityPair.Key, NPS->ASC->GiveAbility(
										   FGameplayAbilitySpec(AbilityPair.Value, 1, AbilityPair.Key, this)
									   )
				);
			}
		}
		CurrentAbilityActions = BaseAbilityActions;
		FString String = "";
		for (auto Element : BaseAbilityActions)
		{
			String += FString::Printf(TEXT("[%d, %s, %s]"), Element.Key, ToCStr(Element.Value.ToString()),
									  ToCStr(Names[Element.Key]));
		}
		UE_LOG(LogActionSystem, Display, TEXT("[NPlayerActionComponent] CurrentAbilityActions: {%s}"),
			   ToCStr(String));


		bSetup = true;
	}
	return;
}
