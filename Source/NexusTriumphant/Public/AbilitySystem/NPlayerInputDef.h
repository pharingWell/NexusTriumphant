// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "NActionHelper.h"
#include "Engine/DataAsset.h"
#include "NPlayerInputDef.generated.h"

/**
 * 
 */
UCLASS()
class NEXUSTRIUMPHANT_API UNPlayerInputDef : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY()
	const TMap<TEnumAsByte<ENAbilityAction>, UInputAction*> BindingMap;
	
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Enqueue Input Action Modifier")
	UInputAction* EnqueueInput{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Input Mapping")
	UInputMappingContext* MappingContext{nullptr};
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Attack Input Action")
	UInputAction* AttackInput{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="First Ability Input Action")
	UInputAction* Ability1Input{nullptr};

	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Second Ability Input Action")
	UInputAction* Ability2Input{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Third Ability Input Action")
	UInputAction* Ability3Input{nullptr};

	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Ultimate Input Action")
	UInputAction* UltimateInput{nullptr};

	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Trait Input Action")
	UInputAction* TraitInput{nullptr};

	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="First Additional Ability Input Action")
	UInputAction* AdditionalAbility1Input{nullptr};
	
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Second Additional Ability Input Action")
	UInputAction* AdditionalAbility2Input{nullptr};
public:
	explicit UNPlayerInputDef(const FObjectInitializer& ObjectInitializer);
	
};
