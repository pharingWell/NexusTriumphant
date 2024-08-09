// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Entities/Player/NChampionDef.h"
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

private:
	/** Ability System Component, Attributes, Effects and Abilities for setup */

	UPROPERTY(VisibleAnywhere, Category="Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	UNChampionDef* ChampionDataAsset;
	
	
	/*UPROPERTY(Replicated, VisibleAnywhere, Category="Abilities")
	UNBaseAttributeSet* StandardAttributes{nullptr};

	
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	UNAbilitySet* InitialAbilitySet{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;
	*/

	
	/** FUNCTIONS */

public:
	ANPlayerState(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	
	// IAbilitySystemInterface
	UNChampionDef* GetChampionDataAsset() const { return ChampionDataAsset; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	// Sets the spec handle with the key Action in CurrentAbilityActions
	// to the spec handle with the key Action in BaseAbilityActions

protected:
	


};
