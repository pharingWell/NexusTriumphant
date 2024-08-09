// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/NActionHelper.h"
#include "GameplayTagContainer.h"
#include "NPlayerSystem.h"
#include "NPlayerActionComponent.generated.h"

class UNPlayerSystem;
class ANPlayerState;
class ANPlayerController;
struct FGameplayTag;

// Should not be made valid
static FGameplayAbilitySpecHandle BlankHandle;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEXUSTRIUMPHANT_API UNPlayerActionComponent : public UActorComponent, public IAbilitySystemInterface,
                                                    public INPlayerSystemInterface
{
	GENERATED_BODY()

protected:
	FGameplayAbilitySpecHandle* CurrentActionSpecHandle;
	FGameplayAbilityActorInfo AbilityActorInfo;
	bool bExecutingQueue;
	TQueue<ENAbilityAction> Queue;

	UPROPERTY()
	TObjectPtr<UNPlayerSystem> NPS;
	bool bNPSValid;
	
	// UPROPERTY()
	// ANPlayerController* NPlayerController;
	// UPROPERTY()
	// ANPlayerState* NPlayerStateRef;
	// UPROPERTY()
	// UAbilitySystemComponent* ASCRef;
	
	TMap<ENAbilityAction, FGameplayAbilitySpecHandle> BaseAbilityActions;
	TMap<ENAbilityAction, FGameplayAbilitySpecHandle> CurrentAbilityActions;

	bool bSetup;
	bool bPlay;
	
public:
	// Sets default values for this component's properties
	UNPlayerActionComponent(const FObjectInitializer& ObjectInitializer);

	// Initialize the PlayerActionComponent with the references needed to run. Called from the PlayerController
	void Setup(ANPlayerState* NPlayerState, ANPlayerController* PlayerController);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		if(bNPSValid)
		{
			return NPS->ASC;
		}
		return nullptr;
	}

	void RevertAbilityAction(ENAbilityAction Action);
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	FGameplayAbilitySpecHandle& GetHandle(ENAbilityAction Action, bool GetBase = false);

	// External function to run an ability action, does not enqueue 
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void ExecuteAction(const ENAbilityAction Action);
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void CancelCurrentAction();

	// External function to enqueue an ability action, may not run action immediately  
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void EnqueueAction(const ENAbilityAction Action);
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void ClearQueue();

	// TODO: hook up so that queue continues. 
	
	UFUNCTION()
	void ActionEnded(const FAbilityEndedData& AbilityEndedData);
	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	//						   FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void SetupNPS(TObjectPtr<UNPlayerSystem> InNPS) override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Internal call for running the ability action
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	bool RunAbilityAction(ENAbilityAction Action);

	void ExecuteQueue();
	void ExecuteQueuedAction();
};
