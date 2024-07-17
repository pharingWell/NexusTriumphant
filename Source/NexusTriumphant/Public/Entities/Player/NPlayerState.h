// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/NAbilitySet.h"
#include "AbilitySystem/Attributes/NBaseAttributeSet.h"

#include "GameFramework/PlayerState.h"
#include "NPlayerState.generated.h"

class UNAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class NEXUSTRIUMPHANT_API ANPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	/** PROPERTIES */

public:
	/** Ability System Component, Attributes, Effects and Abilities for setup */

	UPROPERTY(VisibleAnywhere, Category="Abilities")
	UAbilitySystemComponent* AbilitySystemComponent{nullptr};

	UPROPERTY(Replicated, VisibleAnywhere, Category="Abilities")
	UNBaseAttributeSet* StandardAttributes{nullptr};

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	UNAbilitySet* InitialAbilitySet{nullptr};

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;

	/** FUNCTIONS */
	
public:
	ANPlayerState(const FObjectInitializer& ObjectInitializer);
	
	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	void SetupInitialAbilitiesAndEffects();
	void OnHealthAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;
};
