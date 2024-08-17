// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "AbilitySystem/NActionHelper.h"
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
	TMap<TEnumAsByte<ENAbilityAction>, TObjectPtr<UInputAction>> BindingMap;
	
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Enqueue Input Action Modifier")
	TObjectPtr<UInputAction> EnqueueInput;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Input Mapping")
	TObjectPtr<UInputMappingContext> MappingContext;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Move To Input Action")
	TObjectPtr<UInputAction> MoveToInput;
	
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Attack Input Action")
	TObjectPtr<UInputAction> AttackInput;
	
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="First Ability Input Action")
	TObjectPtr<UInputAction> Ability1Input;

	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Second Ability Input Action")
	TObjectPtr<UInputAction> Ability2Input;
	
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Third Ability Input Action")
	TObjectPtr<UInputAction> Ability3Input;

	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Ultimate Input Action")
	TObjectPtr<UInputAction> UltimateInput;

	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Trait Input Action")
	TObjectPtr<UInputAction> TraitInput;

	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="First Additional Ability Input Action")
	TObjectPtr<UInputAction> AdditionalAbility1Input;
	
	UPROPERTY(EditDefaultsOnly, Category="Input", DisplayName="Second Additional Ability Input Action")
	TObjectPtr<UInputAction> AdditionalAbility2Input;
public:
	explicit UNPlayerInputDef(const FObjectInitializer& ObjectInitializer);

	MAKE A SUBCLASS OF UINPUTACTION?
};
