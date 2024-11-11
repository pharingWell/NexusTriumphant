#pragma once

#include "NActionHelper.generated.h"

class UTexture2D;

DECLARE_LOG_CATEGORY_EXTERN(LogActionSystem, Verbose, All);

UENUM(Blueprintable)
enum ENAbilityAction //Nexus Ability Action: What action the play is taking to activate these things
{
	INVALID = 0 UMETA(DisplayName="Invalid"),
	MOVETO      UMETA(DisplayName="MoveTo"),
	ATTACK      UMETA(DisplayName="Attack"),
	ABILITY1    UMETA(DisplayName="Ability1"),
	ABILITY2    UMETA(DisplayName="Ability2"),
	ABILITY3    UMETA(DisplayName="Ability3"),
	ULTIMATE    UMETA(DisplayName="AbilityUltimate"),
	TRAIT       UMETA(DisplayName="AbilityTrait"),
	ADDT1       UMETA(DisplayName="AbilityAddt1"),
	ADDT2		UMETA(DisplayName="AbilityAddt2"),
};

USTRUCT(Blueprintable, BlueprintType)
struct NEXUSTRIUMPHANT_API FNAbilityIconDef
{
	GENERATED_BODY()
public:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	FText Keybinding;
};

