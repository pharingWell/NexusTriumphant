// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "NDA_Champion.generated.h"

/**
 * 
 */

class UGameplayAbility;

// the amount of items in ENGameplayAbility
#define ENGA_ENUM_ITEMS 10
#define ENGA_ADDITIONAL_ABILITIES 4

UENUM()
enum ENGameplayAbility
{
	ATTACK,
	ABILITY1,
	ABILITY2,
	ABILITY3,
	ABILITYULTIMATE,
	ABILITYTRAIT,
	ADDT1,
	ADDT2,
	ADDT3,
	ADDT4,
};

UCLASS()
class NEXUSTRIUMPHANT_API UNDA_Champion : public UDataAsset
{
	GENERATED_BODY()
	
public:
	void BeginPlay();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNGameplayAbility> AttackClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNGameplayAbility> Ability1Class;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNGameplayAbility> Ability2Class;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNGameplayAbility> Ability3Class;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNGameplayAbility> AbilityUltimateClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNGameplayAbility> AbilityTraitClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UNGameplayAbility>> GainedAbilitiesClasses;

	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute ChampionStats;
	
public:
	
};
