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
	UPROPERTY(EditDefaultsOnly)
	UNGameplayAbility* Ability1;

	UPROPERTY(EditDefaultsOnly)
	UNGameplayAbility* Ability2;

	UPROPERTY(EditDefaultsOnly)
	UNGameplayAbility* Ability3;

	UPROPERTY(EditDefaultsOnly)
	UNGameplayAbility* AbilityUltimate;

	UPROPERTY(EditDefaultsOnly)
	UNGameplayAbility* AbilityTrait;

	UPROPERTY(EditDefaultsOnly)
	TArray<UNGameplayAbility*> GainedAbilities;

public:
	
};
