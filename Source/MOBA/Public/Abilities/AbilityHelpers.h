#pragma once

#include "CoreMinimal.h"
DECLARE_LOG_CATEGORY_EXTERN(LogAbilitySystem, Warning, All);
#define FULLNAME_CSTR(Object) ToCStr(Object->GetFullName())
#define UE_LOG_ABILITY(Verbosity, String, ...) UE_LOG(LogAbilitySystem, Type::Verbosity, String, ##__VA_ARGS__);

#define UE_LOG_ABILITY_CAUTION(String, Object)                                                                   \
	UE_LOG_ABILITY(Display, TEXT("[Ability System] Caution: %s for %s"), ToCStr(String), FULLNAME_CSTR(Object)); \
	GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow,                                                    \
		FString::Printf(TEXT("[Ability System] Caution: %s for %s"), ToCStr(String), ToCStr(Object->GetName()))      \
	);

class AbilityHelpers
{
};
