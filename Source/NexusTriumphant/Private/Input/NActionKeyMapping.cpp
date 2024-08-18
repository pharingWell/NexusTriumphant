/*
#include "Input/NActionKeyMapping.h"
#include "HAL/IConsoleManager.h"
#include "EnhancedActionKeyMapping.h"

FNActionKeyMapping::FNActionKeyMapping(const FNActionEnumStruct& InActionEnum, const FKey InKey)
	:
	Super(InActionEnum.Action, InKey), EnumStruct(InActionEnum)
{}

bool FNActionKeyMapping::operator==(const FNActionKeyMapping& Other) const
{
	return (EnumStruct.Action == Other.EnumStruct.Action &&
			Key == Other.Key &&
			EnumStruct.Enum == Other.EnumStruct.Enum &&
			Triggers == Other.Triggers &&
			Modifiers == Other.Modifiers);
}
*/
