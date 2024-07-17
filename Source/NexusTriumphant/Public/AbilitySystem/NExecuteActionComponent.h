// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Entities/Player/NPlayerCharacter.h"
#include "NExecuteActionComponent.generated.h"


struct FGameplayAbilitySpecHandle;
class UNGameplayAbility;
class ANPlayerCharacter;
/**
 * 
 */

static FGameplayAbilitySpecHandle INVALID_GA_SPEC_HANDLE;

UCLASS()
class NEXUSTRIUMPHANT_API UNExecuteActionComponent : public UActorComponent, public IAbilitySystemInterface 
{
	GENERATED_BODY()
	
public:
	
protected:
	FGameplayAbilitySpecHandle CurrentAction;
	FGameplayAbilityActorInfo AbilityActorInfo;
	bool bExecutingQueue;
	TQueue<FGameplayAbilitySpecHandle> Queue;
	ANPlayerCharacter* AvatarActor;
	ANPlayerState* OwningState;

	
public:
	UNExecuteActionComponent(const FObjectInitializer& ObjectInitializer);
	virtual void InitializeComponent() override;

	void ExecuteAction(FGameplayAbilitySpecHandle& Action);
	void CancelCurrentAction();
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void EnqueueAction(const FGameplayAbilitySpecHandle& Action);
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void ClearQueue();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		if(IsValid(AvatarActor))
			return AvatarActor->GetAbilitySystemComponent();
		return nullptr;
	}
	void SetPlayerState(ANPlayerState* NewPlayerState)
	{
		OwningState = NewPlayerState;
	}
private:
	void ExecuteQueue();

};
