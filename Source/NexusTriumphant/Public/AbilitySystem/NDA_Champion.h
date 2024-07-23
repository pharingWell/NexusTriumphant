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



UCLASS()
class NEXUSTRIUMPHANT_API UNDA_Champion : public UDataAsset
{
	GENERATED_BODY()
	
public:
	void BeginPlay();

	UPROPERTY(EditDefaultsOnly, DisplayName="Attack Gameplay Ability")
	TSubclassOf<UNGameplayAbility> AttackClass;
	
	UPROPERTY(EditDefaultsOnly, DisplayName="First Gameplay Ability")
	TSubclassOf<UNGameplayAbility> Ability1Class;

	UPROPERTY(EditDefaultsOnly, DisplayName="Second Gameplay Ability")
	TSubclassOf<UNGameplayAbility> Ability2Class;

	UPROPERTY(EditDefaultsOnly, DisplayName="Third Gameplay Ability")
	TSubclassOf<UNGameplayAbility> Ability3Class;

	UPROPERTY(EditDefaultsOnly, DisplayName="Ultimate Gameplay Ability")
	TSubclassOf<UNGameplayAbility> UltimateClass;

	UPROPERTY(EditDefaultsOnly, DisplayName="Trait Gameplay Ability")
	TSubclassOf<UNGameplayAbility> TraitClass;

	UPROPERTY(EditDefaultsOnly, DisplayName="First Additional Gameplay Ability")
	TSubclassOf<UNGameplayAbility> AdditionalAbilities1Class;

	UPROPERTY(EditDefaultsOnly, DisplayName="Second Additional Gameplay Ability")
	TSubclassOf<UNGameplayAbility> AdditionalAbilities2Class;

	UPROPERTY(EditDefaultsOnly, DisplayName="Third Additional Gameplay Ability")
	TSubclassOf<UNGameplayAbility> AdditionalAbilities3Class;

	UPROPERTY(EditDefaultsOnly, DisplayName="Fourth Additional Gameplay Ability")
	TSubclassOf<UNGameplayAbility> AdditionalAbilities4Class;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute ChampionStats;
	
public:
	
};
