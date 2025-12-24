

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystem/NActionHelper.h"
#include "Components/ActorComponent.h"
#include "NPlayerActionComponent.generated.h"

// Should not be made valid

class ANPlayerController;
class ANPlayerState;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FNAbilityActionEntry
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	ENAbilityAction AbilityAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	FGameplayEventData EventData;
};

/*
 * Handles action input from the user and routes is as needed, either to the queue or directly.
 * Directly interacts with the Ability System Component
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEXUSTRIUMPHANT_API UNPlayerActionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	FGameplayAbilitySpecHandle CurrentActionSpecHandle;
	FGameplayAbilityActorInfo AbilityActorInfo;
	bool bExecutingQueue;
	TQueue<FNAbilityActionEntry> Queue;

	UPROPERTY()
	TObjectPtr<ANPlayerController> NPlayerController;
	UPROPERTY()
	TObjectPtr<ANPlayerState> NPlayerState;
	UPROPERTY()
	UAbilitySystemComponent* ASCRef;

	bool bSetup;
	bool bPlay;
	
public:
	// Sets default values for this component's properties
	UNPlayerActionComponent(const FObjectInitializer& ObjectInitializer);
	void Setup(const TObjectPtr<ANPlayerState>& InPlayerState, const TObjectPtr<ANPlayerController>& InPlayerController);
	
	void ApplyInput(ENAbilityAction InputUsed, ENAbilityCastMode CastMode,
	                bool bIsEnqueueing);

	// Calls player controller function to run an ability action, does not enqueue 
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	bool ExecuteAction(const FNAbilityActionEntry& AbilityActionEntry);

	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void CancelCurrentAction();

	// External function to enqueue an ability action, may not run action immediately  
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void EnqueueAction(const FNAbilityActionEntry& AbilityActionEntry);
	
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

	void ExecuteQueue();
	void ExecuteQueuedAction();
};
