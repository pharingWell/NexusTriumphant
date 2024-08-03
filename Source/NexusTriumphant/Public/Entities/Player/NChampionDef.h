// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "AbilitySystem/NGameplayAbility.h"
#include "AbilitySystem/NActionHelper.h"
#include "Engine/DataAsset.h"

#include "NChampionDef.generated.h"

/*USTRUCT()
struct FNActionHandlerFunctions
{
	GENERATED_BODY()
	FNActionHandlerFunctions(
					const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampionDef> InputStartedFunc,
					const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampionDef> InputTriggeredFunc,
					const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampionDef> InputReleasedFunc):
		InputStartedFunc(InputStartedFunc),
		InputTriggeredFunc(InputTriggeredFunc),
		InputReleasedFunc(InputReleasedFunc)
	{}
	UPROPERTY()
	FEnhancedInputActionHandlerSignature
	const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampionDef> InputStartedFunc;
	const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampionDef> InputTriggeredFunc;
	const FEnhancedInputActionHandlerSignature::TMethodPtr<UNChampionDef> InputReleasedFunc;
};*/


class UGameplayAbility;

/**
 * Champion Specification
 * These classes act to hold the definitions for which gameplay abilities are associated with which Champion
 * The classes will also contain stat definitions in the future
 */
UCLASS()
class NEXUSTRIUMPHANT_API UNChampionDef : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Attack Gameplay Ability")
	TSubclassOf<UGameplayAbility> AttackClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="First Gameplay Ability")
	TSubclassOf<UGameplayAbility> Ability1Class;

	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Second Gameplay Ability")
	TSubclassOf<UGameplayAbility> Ability2Class;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Third Gameplay Ability")
	TSubclassOf<UGameplayAbility> Ability3Class;

	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Ultimate Gameplay Ability")
	TSubclassOf<UGameplayAbility> UltimateClass;

	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Trait Gameplay Ability")
	TSubclassOf<UGameplayAbility> TraitClass;

	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="First Additional Gameplay Ability")
	TSubclassOf<UGameplayAbility> AdditionalAbility1Class;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Second Additional Gameplay Ability")
	TSubclassOf<UGameplayAbility> AdditionalAbility2Class;

public:	
	
	FGameplayAttribute ChampionStats;
	
	UPROPERTY()
	TMap<TEnumAsByte<ENAbilityAction>, TSubclassOf<UGameplayAbility>> AbilityMap;

private:
	//TMap<ENAbilityAction, FNActionHandlerFunctions> HandlerFunctionMap;

	/** FUNCTIONS */
	
public:
	UNChampionDef(const FObjectInitializer& ObjectInitializer);
	
	/* void SetBinding(UEnhancedInputComponent* EnhancedInputComponent, ENAbilityAction EnumKey,
	                const UInputAction* InputAction);
*/
	// void SetBinding(UEnhancedInputComponent* EnhancedInputComponent, ENAbilityAction EnumKey,
	//                 const UInputAction* InputAction);
	// UFUNCTION()
	// void BindInputs(UInputComponent* InputComponent, TMap<ENAbilityAction, UInputAction*> InputMap);

};
