// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/NActionHelper.h"
#include "Components/ActorComponent.h"
#include "NPlayerActionComponent.generated.h"

// Should not be made valid
static FGameplayAbilitySpecHandle BlankHandle;
class ANPlayerController;
class ANPlayerState;
class UAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEXUSTRIUMPHANT_API UNPlayerActionComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	FGameplayAbilitySpecHandle CurrentActionSpecHandle;
	FGameplayAbilityActorInfo AbilityActorInfo;
	bool bExecutingQueue;
	TQueue<ENAbilityAction> Queue;

	UPROPERTY()
	ANPlayerController* NPlayerController;
	UPROPERTY()
	ANPlayerState* NPlayerState;
	UPROPERTY()
	UAbilitySystemComponent* ASCRef;
	TMap<ENAbilityAction, FGameplayAbilitySpecHandle> BaseAbilityActions;
	TMap<ENAbilityAction, FGameplayAbilitySpecHandle> CurrentAbilityActions;

	bool bSetup;
	bool bPlay;
	
public:
	// Sets default values for this component's properties
	UNPlayerActionComponent(const FObjectInitializer& ObjectInitializer);
	void Setup(ANPlayerState* InPlayerState, ANPlayerController* InPlayerController);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
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
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Internal call for running the ability action
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	bool RunAbilityAction(ENAbilityAction Action);
	void ExecuteQueue();
	void ExecuteQueuedAction();
};
