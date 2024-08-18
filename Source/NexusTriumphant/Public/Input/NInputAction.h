// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "AbilitySystem/NActionHelper.h"
#include "NInputAction.generated.h"

/**
 * 
 */
UCLASS(HideCategories=("Input|Action", "Input"))
class NEXUSTRIUMPHANT_API UNInputAction : public UInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Action", meta=(DisplayName="Enum"))
	TEnumAsByte<ENAbilityAction> ActionEnum;
};
