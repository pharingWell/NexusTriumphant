#include "S:\Unreal\Engine\Intermediate\Build\Win64\x64\UnrealEditorGPF\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.Cpp20.h"
#include "AbilitySystem/NActionHelper.h"

void FNChampionActions::SetActions(TMap<ENGameplayAbility, TSubclassOf<UNGameplayAbility>> ActionMap)
{
	for (auto Pair : ActionMap)
	{
		SetAction(Pair.Key, Pair.Value);
	}
}

void FNChampionActions::SetAction(ENGameplayAbility EnumKey, const TSubclassOf<UNGameplayAbility>& AbilityClass)
{
	AbilityMap.Add(EnumKey, AbilityClass);
}

void FNChampionActions::GiveAbilities(UObject* CallingObject, UAbilitySystemComponent* ASC) 
{
	if (!IsValid(ASC))
	{
		return;
	}

	GASpecHandles.Empty();
	for(int i = 0; i < ENGA_ENUM_ITEMS; i++){ GASpecHandles.Add(Blank); }
	for (auto Pair : AbilityMap)
	{
		if(IsValid(Pair.Value))
		{
			GASpecHandles[Pair.Key] = ASC->GiveAbility(FGameplayAbilitySpec(Pair.Value, 1, Pair.Key, CallingObject));
		}else
		{
			
		}
	}
	
}

const TArray<FGameplayAbilitySpecHandle>& FNChampionActions::GetSpecHandles(OUT bool& bSucceededOut)
{
	if(GASpecHandles.IsEmpty() || GASpecHandles.Num() < ENGA_ENUM_ITEMS)
	{
		bSucceededOut = false;
		return InvalidArray;
	}

	bSucceededOut = true;
	return GASpecHandles;
	
}