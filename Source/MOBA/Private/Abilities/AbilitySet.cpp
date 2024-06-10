// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilitySet.h"

#include "AbilitySystemComponent.h"

const TSet<FAbilityInputInfo>& UAbilitySet::GetInputAbilities() const
{
	return InputAbilities;
}

TArray<FGameplayAbilitySpecHandle> UAbilitySet::GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const
{
	check(AbilitySystemComponent);

	TArray<FGameplayAbilitySpecHandle> handles;
	handles.Reserve(AbilitySetItems.Num());

	for (const FAbilitySetItem AbilitySetItem : AbilitySetItems)
	{
		handles.Emplace(AbilitySystemComponent->GiveAbility
			(FGameplayAbilitySpec(
				AbilitySetItem.GameplayAbility,
				0,
				AbilitySetItem.Input,
				AbilitySystemComponent->GetOwnerActor()
				)
			)
		);
	}
	
	return handles;
}

#if WITH_EDITOR
void UAbilitySet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FProperty* Property = PropertyChangedEvent.Property;
	if (Property && Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAbilitySet, InputAbilities) && !InputAbilities.IsEmpty())
	{
		TArray<FAbilityInputInfo> InputAbilitiesArray = InputAbilities.Array();
		InputAbilities.Reset();
  
		for (int32 i = 0; i < InputAbilitiesArray.Num(); ++i)
		{
			FAbilityInputInfo& Info = InputAbilitiesArray[i];
			Info.InputID = i;
			InputAbilities.Add(Info);
		}
	}
}
#endif