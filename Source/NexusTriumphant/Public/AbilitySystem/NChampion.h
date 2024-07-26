// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "NGameplayAbility.h"
#include "AbilitySystem/NActionHelper.h"
#include "Engine/DataAsset.h"
#include "NChampion.generated.h"

/**
 * 
 */
USTRUCT()
struct FNActionHandlerFunctions
{
	GENERATED_BODY()
	FNActionHandlerFunctions(
					const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampion> InputStartedFunc,
					const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampion> InputTriggeredFunc,
					const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampion> InputReleasedFunc):
		InputStartedFunc(InputStartedFunc),
		InputTriggeredFunc(InputTriggeredFunc),
		InputReleasedFunc(InputReleasedFunc)
	{}
	UPROPERTY()
	const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampion> InputStartedFunc;
	const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampion> InputTriggeredFunc;
	const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampion> InputReleasedFunc;
};


class UGameplayAbility;

UCLASS()
class NEXUSTRIUMPHANT_API UNChampion : public UDataAsset
{
	GENERATED_BODY()
	
public:
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
	TSubclassOf<UNGameplayAbility> AdditionalAbility1Class;

	UPROPERTY(EditDefaultsOnly, DisplayName="Second Additional Gameplay Ability")
	TSubclassOf<UNGameplayAbility> AdditionalAbility2Class;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute ChampionStats;
	
	UPROPERTY()
	TMap<ENAbilityAction, TSubclassOf<UNGameplayAbility>> AbilityMap;
	UPROPERTY()
	TMap<ENAbilityAction, UInputAction*> BindingMap;
private:
	TMap<ENAbilityAction, FNActionHandlerFunctions> HandlerFunctionMap;

	/** FUNCTIONS */
	
public:
	UNChampion::UNChampion() :
	AbilityMap({
		{ENAbilityAction::ATTACK, AttackClass},
		{ENAbilityAction::ABILITY1, Ability1Class},
		{ENAbilityAction::ABILITY2, Ability2Class},
		{ENAbilityAction::ABILITY3, Ability3Class},
		{ENAbilityAction::ULTIMATE, UltimateClass},
		{ENAbilityAction::TRAIT, TraitClass},
		{ENAbilityAction::ADDT1, AdditionalAbility1Class},
		{ENAbilityAction::ADDT2, AdditionalAbility2Class},
	})
	{}

	// void SetBinding(UEnhancedInputComponent* EnhancedInputComponent, ENAbilityAction EnumKey,
	//                 const UInputAction* InputAction);
	// UFUNCTION()
	// void BindInputs(UInputComponent* InputComponent, TMap<ENAbilityAction, UInputAction*> InputMap);

};
