

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystem/NActionHelper.h"
#include "Components/ActorComponent.h"
#include "NPlayerActionComponent.generated.h"

// Should not be made valid
static FGameplayAbilitySpecHandle BlankHandle;
class ANPlayerController;
class ANPlayerState;
class UAbilitySystemComponent;

USTRUCT()
struct FNActionQueueEntry
{
	GENERATED_BODY()
	UPROPERTY()
	ENAbilityAction AbilityAction;
	UPROPERTY()
	FGameplayEventData EventData;
};

/*
 * Handles action input from the user and routes is as needed, either to the queue or directly.
 * Directly interacts with the Ability System Component
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEXUSTRIUMPHANT_API UNPlayerActionComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	FGameplayAbilitySpecHandle CurrentActionSpecHandle;
	FGameplayAbilityActorInfo AbilityActorInfo;
	bool bExecutingQueue;
	TQueue<FNActionQueueEntry> Queue;

	UPROPERTY()
	TObjectPtr<ANPlayerController> NPlayerController;
	UPROPERTY()
	TObjectPtr<ANPlayerState> NPlayerState;
	UPROPERTY()
	UAbilitySystemComponent* ASCRef;
	TMap<ENAbilityAction, FGameplayAbilitySpecHandle> BaseAbilityActions;
	TMap<ENAbilityAction, FGameplayAbilitySpecHandle> CurrentAbilityActions;

	bool bSetup;
	bool bPlay;
	
public:
	// Sets default values for this component's properties
	UNPlayerActionComponent(const FObjectInitializer& ObjectInitializer);
	void Setup(const TObjectPtr<ANPlayerState>& InPlayerState, const TObjectPtr<ANPlayerController>& InPlayerController);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void RevertAbilityAction(ENAbilityAction Action);
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	FGameplayAbilitySpecHandle& GetHandle(ENAbilityAction Action, bool GetBase = false);
	void ApplyInput(ENAbilityAction InputUsed, ENAbilityCastMode CastMode,
	                bool bIsEnqueueing);

	// External function to run an ability action, does not enqueue 
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	bool ExecuteAction(ENAbilityAction Action, const FGameplayEventData& EventData, bool ClearQueue = true);

	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void CancelCurrentAction();

	// External function to enqueue an ability action, may not run action immediately  
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void EnqueueAction(ENAbilityAction Action, const FGameplayEventData& EventData);
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void ClearQueue();

	// TODO: hook up so that queue continues. 
	
	UFUNCTION()
	void ActionEnded(const FAbilityEndedData& AbilityEndedData);
	FGameplayAbilityTargetDataHandle MakeTargetDataHandleFromHitResult(const FHitResult& HitResult);
	FGameplayAbilityTargetDataHandle MakeTargetDataHandleFromHitResults(const TArray<FHitResult>& HitResults);
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	//						   FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Internal call for running the ability action
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	bool RunAbilityAction(ENAbilityAction Action, const FGameplayEventData& EventData);
	void ExecuteQueue();
	void ExecuteQueuedAction();
};
