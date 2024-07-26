#include "AbilitySystem/NActionHelper.h"



DEFINE_LOG_CATEGORY(LogActionSystem);

FNActionContainer::FNActionContainer(UObject* OwningObject) : OwningObject(OwningObject)
{}

bool FNActionContainer::ContainerIsValid() const
{
	return bValidityAbilities;
}

/*
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
*/

void FNActionContainer::SetAction(ENAbilityAction EnumKey, const TSubclassOf<UNGameplayAbility>& AbilityClass)
{
	AbilityMap.Add(EnumKey, AbilityClass);
	if(!bValidityAbilities) { bValidityAbilities = true; }
}

void FNActionContainer::SetActions(const TMap<ENAbilityAction, TSubclassOf<UNGameplayAbility>>& ActionMap)
{
	for (auto Pair : ActionMap)
	{
		SetAction(Pair.Key, Pair.Value);
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