// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

# include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "EnhancedActionKeyMapping.h"
#include "AbilitySystem/NActionHelper.h"


#include "NInputMappingContext.generated.h"

struct FKey;


class UNInputAction;


USTRUCT(BlueprintType)
struct FNEnhancedEnumMapping
{
	GENERATED_BODY()
	FNEnhancedEnumMapping(UInputAction* InAction  = nullptr, TEnumAsByte<ENAbilityAction> InEnum = ENAbilityAction::INVALID,
		TArray<FEnhancedActionKeyMapping> InMappings = {})
		: Action(InAction), Enum(InEnum)
	{
		
	}
	/** Action to be affected by the key  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<const UInputAction> Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TEnumAsByte<ENAbilityAction> Enum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FEnhancedActionKeyMapping> KeyMappings;
};

/**
* UInputMappingContext : A collection of key to action mappings for a specific input context
* Could be used to:
*	Store predefined controller mappings (allow switching between controller config variants). TODO: Build a system allowing redirects of UInputMappingContexts to handle this.
*	Define per-vehicle control mappings
*	Define context specific mappings (e.g. I switch from a gun (shoot action) to a grappling hook (reel in, reel out, disconnect actions).
*	Define overlay mappings to be applied on top of existing control mappings (e.g. Hero specific action mappings in a NexusTriumphant)
*/

UCLASS(BlueprintType, config = Input, PrioritizeCategories=("EnumMappings"))
class NEXUSTRIUMPHANT_API UNInputMappingContext : public UInputMappingContext
{
	GENERATED_BODY()

	friend class FInputContextDetails;
	friend class FActionMappingsNodeBuilderEx;
protected:
	// List of key to action mappings.
	UPROPERTY(config, BlueprintReadOnly, EditAnywhere, Category = "EnumMappings", meta = (DisplayName="KeyMappings with Enum"))
	TArray<FNEnhancedEnumMapping> NMappingArray;
	TMap<ENAbilityAction, const TArray<FEnhancedActionKeyMapping>> NEnumMappingMap;
	
public:
	UNInputMappingContext();
	void CreateUnderlyingMapping();
	virtual void PostLoad() override;
	void UpdateKeyMappingActions();
#if WITH_EDITOR
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	/**
	* Mapping accessors.
	* Note: Use UEnhancedInputLibrary::RequestRebuildControlMappingsForContext to invoke changes made to an FNActionKeyMapping
	*/
	const TArray<FNEnhancedEnumMapping>& GetNMappings() const { return NMappingArray; }
	FNEnhancedEnumMapping& GetNMapping(TArray<FNEnhancedEnumMapping>::SizeType Index) { return NMappingArray[Index]; }

};
