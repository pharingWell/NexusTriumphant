// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/NInputMappingContext.h"

#include "EnhancedInputLibrary.h"
#include "EnhancedInputModule.h"
#include "PlayerMappableKeySettings.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UNInputMappingContext::UNInputMappingContext()
	:
	Super()
{
}

#define LOCTEXT_NAMESPACE "InputMappingContext"
#if WITH_EDITOR
EDataValidationResult UNInputMappingContext::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult 	Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);
	if(Mappings.Num() > 0)
	{
		Context.AddError(LOCTEXT("MappingsNotEmpty", "Mappings cannot have any elements!"));
		Result = EDataValidationResult::Invalid;
	}
	Result = CombineDataValidationResults(Result, NMappingArray.Action->IsDataValid(Context));
	for (const FEnhancedActionKeyMapping& Mapping : NMappingArray.Mappings)
	{
		EDataValidationResult MappingResult = Mapping.IsDataValid(Context);
		if(Mapping.Action != NMappingArray.Action)
		{
			Context.AddError(LOCTEXT("ActionMismatch", "Enum Mappings Action does not match individual mapping Action!"));
			MappingResult = CombineDataValidationResults(MappingResult, EDataValidationResult::Invalid);
		}
		Result = CombineDataValidationResults(Result, MappingResult);
	}
	return Result;
}
#endif	// WITH_EDITOR
#undef LOCTEXT_NAMESPACE 

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