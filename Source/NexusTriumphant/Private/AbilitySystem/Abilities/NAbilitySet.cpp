// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/NAbilitySet.h"

#include "Player/NAbilitySystemComponent.h"

const TSet<FAbilityInputInfo>& UNAbilitySet::GetInputAbilities() const
{
	return InputAbilities;
}

TArray<FGameplayAbilitySpecHandle> UNAbilitySet::GrantAbilitiesToAbilitySystem(UNAbilitySystemComponent* NASC) const
{
	check(NASC);

	TArray<FGameplayAbilitySpecHandle> handles;
	handles.Reserve(AbilitySetItems.Num());

	for (const FAbilitySetItem AbilitySetItem : AbilitySetItems)
	{
		handles.Emplace(NASC->GiveAbility
			(FGameplayAbilitySpec(
				AbilitySetItem.GameplayAbility,
				0,
				AbilitySetItem.Input,
				NASC->GetOwnerActor()
				)
			)
		);
	}
	
	return handles;
}

#if WITH_EDITOR
void UNAbilitySet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FProperty* Property = PropertyChangedEvent.Property;
	if (Property && Property->GetFName() == GET_MEMBER_NAME_CHECKED(UNAbilitySet, InputAbilities) && !InputAbilities.IsEmpty())
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