// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NChampion.h"

#include "AbilitySystem/NActionHelper.h"




void UNChampion::SetBinding(UEnhancedInputComponent EnhancedInputComponent*, ENAbilityAction EnumKey, const UInputAction* InputAction)
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


void UNChampion::BindInputs(UInputComponent* InputComponent, TMap<ENAbilityAction, UInputAction*> InputMap)
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
}


