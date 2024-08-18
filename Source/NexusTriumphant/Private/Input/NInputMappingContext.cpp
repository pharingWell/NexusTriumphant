// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/NInputMappingContext.h"

#include "EnhancedInputLibrary.h"
#include "EnhancedInputModule.h"
#include "PlayerMappableKeySettings.h"


UNInputMappingContext::UNInputMappingContext()
	:
	Super()
{
}

FEnhancedActionKeyMapping& UNInputMappingContext::NMapKey(const UInputAction* InAction, FKey ToKey,
                                                          TEnumAsByte<ENAbilityAction> Enum)
{
	IEnhancedInputModule::Get().GetLibrary()->RequestRebuildControlMappingsUsingContext(this);
	return NMappingArray.Mappings.Add_GetRef(FEnhancedActionKeyMapping(InAction, ToKey));
}

void UNInputMappingContext::NUnmapKey(const UInputAction* Action, FKey Key, TEnumAsByte<ENAbilityAction> InEnum)
{
	if(InEnum != NMappingArray.Enum)
	{
		return;;
	}
	int32 MappingIdx = NMappingArray.Mappings.IndexOfByPredicate(
		[&Action, &Key, &InEnum](const FEnhancedActionKeyMapping& Other)
		{ return Other.Action == Action && Other.Key == Key; }
	);
	if (MappingIdx != INDEX_NONE)
	{
		NMappingArray.Mappings.RemoveAtSwap(MappingIdx);	// TODO: Preserve order?
		IEnhancedInputModule::Get().GetLibrary()->RequestRebuildControlMappingsUsingContext(this);
	}
}

void UNInputMappingContext::NUnmapAllKeysFromAction(const UInputAction* Action)
{
	int32 Found = NMappingArray.Mappings.RemoveAllSwap([&Action](const FEnhancedActionKeyMapping& Entry) { return Entry.Action == Action; });
	if (Found > 0)
	{
		IEnhancedInputModule::Get().GetLibrary()->RequestRebuildControlMappingsUsingContext(this);
	}
}

void UNInputMappingContext::NUnmapAll()
{
	if (NMappingArray.Mappings.Num())
	{
		NMappingArray.Mappings.Empty();
		IEnhancedInputModule::Get().GetLibrary()->RequestRebuildControlMappingsUsingContext(this);
	}
}