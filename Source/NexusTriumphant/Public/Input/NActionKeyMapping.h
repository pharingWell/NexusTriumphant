#pragma once
#include "EnhancedActionKeyMapping.h"
#include "InputAction.h"
#include "Input/NInputAction.h"
#include "AbilitySystem/NActionHelper.h"
#include "NActionKeyMapping.generated.h"

class UNInputAction;
class UInputModifier;
class UInputTrigger;
class UPlayerMappableKeySettings;

USTRUCT(BlueprintType)
struct FNActionEnumStruct
{
	GENERATED_BODY()
	FNActionEnumStruct(const UNInputAction* InAction = nullptr, const TEnumAsByte<ENAbilityAction> InEnum = ENAbilityAction::INVALID)
		: Action(InAction), Enum(InEnum)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<const UInputAction> Action = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TEnumAsByte<ENAbilityAction> Enum = ENAbilityAction::INVALID;
	
};

/**
 * Defines a mapping between a key activation and the resulting enhanced action
 * An key could be a button press, joystick axis movement, etc.
 * An enhanced action could be MoveForward, Jump, Fire, etc.
 *
**/
USTRUCT(BlueprintType)
struct NEXUSTRIUMPHANT_API FNActionKeyMapping
{
	friend class UInputMappingContext;
	friend class FEnhancedActionMappingCustomization;
	
	GENERATED_BODY()

	FNActionKeyMapping(const FNActionEnumStruct& InActionEnum = FNActionEnumStruct(), const FKey InKey = EKeys::Invalid);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FNActionEnumStruct EnumStruct = FNActionEnumStruct();

	/** Identical comparison, including Triggers and Modifiers current inner values. */
	bool operator==(const FNActionKeyMapping& Other) const;

};
