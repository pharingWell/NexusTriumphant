// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Player/NChampionDef.h"

#include "EnhancedInputComponent.h"
#include "AbilitySystem/NActionHelper.h"


UNChampionDef::UNChampionDef(const FObjectInitializer&)
{
	AbilityMap= {
			{ENAbilityAction::ATTACK, AttackClass},
			{ENAbilityAction::ABILITY1, Ability1Class},
			{ENAbilityAction::ABILITY2, Ability2Class},
			{ENAbilityAction::ABILITY3, Ability3Class},
			{ENAbilityAction::ULTIMATE, UltimateClass},
			{ENAbilityAction::TRAIT, TraitClass},
			{ENAbilityAction::ADDT1, AdditionalAbility1Class},
			{ENAbilityAction::ADDT2, AdditionalAbility2Class},
		};
	
}

/*void UNChampionDef::SetBinding(UEnhancedInputComponent EnhancedInputComponent*, ENAbilityAction EnumKey, const UInputAction* InputAction)
{

	if(HandlerFunctionMap.Contains(EnumKey))
	{
		FNActionHandlerFunctions& Binding = HandlerFunctionMap[EnumKey];
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, Binding.InputStartedFunc);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, Binding.InputTriggeredFunc);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, Binding.InputReleasedFunc);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, Binding.InputReleasedFunc);
	}
}


void UNChampionDef::BindInputs(UInputComponent* InputComponent, TMap<ENAbilityAction, UInputAction*> InputMap)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		UE_LOG(LogActionSystem, Error, TEXT("[%s] Failed to find/cast an Enhanced Input Component!"),
			*GetNameSafe(this));
		return;
	}
	for (auto Pair : InputMap)
	{
		UInputAction* Input& = Pair.Value;
		// Setup mouse input events
		SetBinding(EnhancedInputComponent, Pair.Key, Input);
	}
}*/


