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


class UNGameplayAbility;

/**
 * Champion Specification
 * These classes act to hold the definitions for which gameplay abilities are associated with which Champion
 * The classes will also contain stat definitions in the future
 */
UCLASS()
class NEXUSTRIUMPHANT_API UNChampionDef : public UDataAsset
{
	GENERATED_BODY()

public:	
	
	FGameplayAttribute ChampionStats;
	
	
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Attack Gameplay Ability")
	TSubclassOf<UNGameplayAbility> AttackClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="First Gameplay Ability")
	TSubclassOf<UNGameplayAbility> Ability1Class;

	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Second Gameplay Ability")
	TSubclassOf<UNGameplayAbility> Ability2Class;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Third Gameplay Ability")
	TSubclassOf<UNGameplayAbility> Ability3Class;

	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Ultimate Gameplay Ability")
	TSubclassOf<UNGameplayAbility> UltimateClass;

	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Trait Gameplay Ability")
	TSubclassOf<UNGameplayAbility> TraitClass;

	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="First Additional Gameplay Ability")
	TSubclassOf<UNGameplayAbility> AdditionalAbility1Class;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability", DisplayName="Second Additional Gameplay Ability")
	TSubclassOf<UNGameplayAbility> AdditionalAbility2Class;

	UPROPERTY()
	TMap<TEnumAsByte<ENAbilityAction>, TSubclassOf<UNGameplayAbility>> AbilityMap;
private:
	//TMap<ENAbilityAction, FNActionHandlerFunctions> HandlerFunctionMap;

	/** FUNCTIONS */
	
public:
	UNChampionDef(const FObjectInitializer& ObjectInitializer);
	TMap<TEnumAsByte<ENAbilityAction>, TSubclassOf<UNGameplayAbility>>& GetUpdatedAbilityMap();
	/* void SetBinding(UEnhancedInputComponent* EnhancedInputComponent, ENAbilityAction EnumKey,
	                const UInputAction* InputAction);
*/
	// void SetBinding(UEnhancedInputComponent* EnhancedInputComponent, ENAbilityAction EnumKey,
	//                 const UInputAction* InputAction);
	// UFUNCTION()
	// void BindInputs(UInputComponent* InputComponent, TMap<ENAbilityAction, UInputAction*> InputMap);

};
