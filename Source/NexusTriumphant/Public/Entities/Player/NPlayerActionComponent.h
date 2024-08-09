// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "NPlayerSystem.h"
#include "NPlayerActionComponent.generated.h"

struct FNPlayerSystem;
class ANPlayerState;
class ANPlayerController;
struct FGameplayTag;

// Should not be made valid
static FGameplayAbilitySpecHandle BlankHandle;

UCLASS()
class NEXUSTRIUMPHANT_API UNPlayerActionComponent : public UActorComponent, public IAbilitySystemInterface,
                                                    public INPlayerSystemInterface
{
	GENERATED_BODY()

protected:
	FGameplayAbilitySpecHandle* CurrentActionSpecHandle;
	FGameplayAbilityActorInfo AbilityActorInfo;
	bool bExecutingQueue;
	TQueue<ENAbilityAction> Queue;
	FNPlayerSystem* NPS;
	
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


	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		if(NPS->Valid())
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
	
	virtual void SetupNPS(FNPlayerSystem* InNPS) override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Internal call for running the ability action
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	bool RunAbilityAction(ENAbilityAction Action);

	void ExecuteQueue();
	void ExecuteQueuedAction();
};
