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

void UNInputMappingContext::CreateUnderlyingMapping()
{
	Mappings.Empty();
	for (auto Element : NMappingArray)
	{
		NEnumMappingMap.Add(Element.Enum, Element.KeyMappings);
		Mappings.Append(Element.KeyMappings);
	}
	UE_LOG(LogInput, Display, TEXT("[NInputMappingContext] Mapping count: %d"), Mappings.Num());
}

void UNInputMappingContext::PostLoad()
{
	Super::PostLoad();
}

void UNInputMappingContext::UpdateKeyMappingActions()
{
	for (auto& Element : NMappingArray)
	{
		for (auto& Mapping : Element.KeyMappings)
		{
			Mapping.Action = Element.Action; //sends the action down to the mappings
		}
	}
}

#if WITH_EDITOR
void UNInputMappingContext::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateKeyMappingActions();
	Super::PostEditChangeProperty(PropertyChangedEvent);
	CreateUnderlyingMapping();
}

void UNInputMappingContext::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	UpdateKeyMappingActions();
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	CreateUnderlyingMapping();
}

#define LOCTEXT_NAMESPACE "InputMappingContext"
EDataValidationResult UNInputMappingContext::IsDataValid(FDataValidationContext& Context) const
{
	TSet<ENAbilityAction> UsedEnums = {};
	EDataValidationResult 	Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);
	// if(Mappings.Num() > 0)
	// {
	// 	Context.AddError(LOCTEXT("MappingsNotEmpty", "Mappings cannot have any elements!"));
	// 	Result = EDataValidationResult::Invalid;
	// }
	for (const FNEnhancedEnumMapping& EnumMapping : NMappingArray)
	{
		if(EnumMapping.Enum == INVALID)
		{
			Context.AddError(LOCTEXT("InvalidEnum", "Ability Action Enum cannont be set to Invalid"));
			Result = EDataValidationResult::Invalid;
		}else
		{
			if(UsedEnums.Contains(EnumMapping.Enum))
			{
				Context.AddError(LOCTEXT("RepeatedEnum", "Same Ability Action Enum cannot be used multiple times."));
				Result = EDataValidationResult::Invalid;
			}
			else
			{
				UsedEnums.Add(EnumMapping.Enum);
			}
		}
		Result = CombineDataValidationResults(Result, EnumMapping.Action->IsDataValid(Context));
		for (const FEnhancedActionKeyMapping& Mapping : EnumMapping.KeyMappings)
		{
			Result = CombineDataValidationResults(Result, Mapping.IsDataValid(Context));
			if(Mapping.Action != EnumMapping.Action)
			{
				Context.AddError(LOCTEXT("ActionMismatch", "Enum Mappings Action does not match individual mapping Action!"));
				Result = EDataValidationResult::Invalid;
			}
		}
	}
	return Result;
}
#undef LOCTEXT_NAMESPACE 
#endif	// WITH_EDITOR