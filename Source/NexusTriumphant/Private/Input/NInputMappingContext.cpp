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

FNActionKeyMapping& UNInputMappingContext::NMapKey(const UNInputAction* Action, FKey ToKey, TEnumAsByte<ENAbilityAction> Enum)
{
	IEnhancedInputModule::Get().GetLibrary()->RequestRebuildControlMappingsUsingContext(this);
	return NMappings.Add_GetRef(FNActionKeyMapping(FNActionEnumStruct(Action, Enum), ToKey));
}

void UNInputMappingContext::NUnmapKey(const UInputAction* Action, FKey Key, TEnumAsByte<ENAbilityAction> Enum)
{
	int32 MappingIdx = NMappings.IndexOfByPredicate(
		[&Action, &Key, &Enum](const FNActionKeyMapping& Other)
		{ return Other.EnumStruct.Action == Action && Other.Key == Key && Other.EnumStruct.Enum == Enum; }
	);
	if (MappingIdx != INDEX_NONE)
	{
		NMappings.RemoveAtSwap(MappingIdx);	// TODO: Preserve order?
		IEnhancedInputModule::Get().GetLibrary()->RequestRebuildControlMappingsUsingContext(this);
	}
}

void UNInputMappingContext::NUnmapAllKeysFromAction(const UInputAction* Action)
{
	int32 Found = NMappings.RemoveAllSwap([&Action](const FEnhancedActionKeyMapping& Mapping) { return Mapping.Action == Action; });
	if (Found > 0)
	{
		IEnhancedInputModule::Get().GetLibrary()->RequestRebuildControlMappingsUsingContext(this);
	}
}

void UNInputMappingContext::NUnmapAll()
{
	if (NMappings.Num())
	{
		NMappings.Empty();
		IEnhancedInputModule::Get().GetLibrary()->RequestRebuildControlMappingsUsingContext(this);
	}
}