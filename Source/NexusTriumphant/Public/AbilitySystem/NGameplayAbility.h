// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NGameplayAbility.generated.h"

/**
 * 
 */
USTRUCT()
struct FNAbilityInfo
{
	GENERATED_BODY()
	
};

UCLASS()
class NEXUSTRIUMPHANT_API UNGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	FNAbilityInfo AbilityInfo;
};
