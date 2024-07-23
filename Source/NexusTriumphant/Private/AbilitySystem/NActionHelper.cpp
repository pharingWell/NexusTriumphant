#include "AbilitySystem/NActionHelper.h"



DEFINE_LOG_CATEGORY(LogActionSystem);

FNActionContainer::FNActionContainer(UObject* OwningObject) : OwningObject(OwningObject)
{}

bool FNActionContainer::ContainerIsValid(ENActionContainerValidity Validity) const
{
	switch (Validity)
	{
		//this was originally a "bit comparison", but with only two booleans, it was overkill
		case BINDINGS:
			return bValidityBindings;
		case ABILITIES:
			return bValidityAbilities;
		case (BINDINGS & ABILITIES):
			return bValidityBindings & bValidityAbilities;
	}
	return false;
}

void FNActionContainer::SetAction(ENGameplayAbility EnumKey, const TSubclassOf<UNGameplayAbility>& AbilityClass)
{
	AbilityMap.Add(EnumKey, AbilityClass);
	if(!bValidityAbilities) { bValidityAbilities = true; }
}

void FNActionContainer::SetActions(TMap<ENGameplayAbility, TSubclassOf<UNGameplayAbility>> ActionMap)
{
	for (auto Pair : ActionMap)
	{
		SetAction(Pair.Key, Pair.Value);
	}

}

template <class T>
void FNActionContainer::SetBinding(ENGameplayAbility EnumKey, FNActionBinding<T> Binding)
{
	BindingMap<T>.Add(EnumKey, Binding);
	// Uses bitmask for better validity checking
	if(!bValidityBindings){ bValidityBindings = true; }
}

template <class T>
void FNActionContainer::SetBindings(TMap<ENGameplayAbility, FNActionBinding<T>> BindingMap)
{
	for (auto Pair : BindingMap)
	{
		SetBinding(Pair.Key, Pair.Value);
	}
}


template <class T>
void FNActionContainer::GiveInputs(UInputComponent* InputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		UE_LOG(LogActionSystem, Error, TEXT("[%s] Failed to find/cast an Enhanced Input Component!"),
			*GetNameSafe(OwningObject));
		return;
	}
	for (auto Pair : BindingMap<T>)
	{
		// Setup mouse input events
		FNActionBinding<T>& Binding = Pair.Value;
		EnhancedInputComponent->BindAction(Binding.InputAction, ETriggerEvent::Started, OwningObject, &Binding.InputStartedFunc);
		EnhancedInputComponent->BindAction(Binding.InputAction, ETriggerEvent::Triggered, OwningObject, &Binding.InputTriggeredFunc);
		EnhancedInputComponent->BindAction(Binding.InputAction, ETriggerEvent::Completed, OwningObject, &Binding.InputReleasedFunc);
		EnhancedInputComponent->BindAction(Binding.InputAction, ETriggerEvent::Canceled, OwningObject, &Binding.InputReleasedFunc);
	}
}

void FNActionContainer::GiveAbilities(UAbilitySystemComponent* ASC) 
{
	if (!IsValid(ASC))
	{
		UE_LOG(LogActionSystem, Error, TEXT("[%s] Failed to find Ability System Componenet!"),
			*GetNameSafe(OwningObject));
		return;
	}
	GASpecHandles.Empty();
	for (auto Pair : AbilityMap)
	{
		if(IsValid(Pair.Value))
		{
			GASpecHandles.Add(Pair.Key,ASC->GiveAbility(FGameplayAbilitySpec(Pair.Value, 1, Pair.Key, OwningObject)));
		}
	}
	
}