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
	TArray<FEnhancedActionKeyMapping> Mappings;

	
	
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

protected:
	// List of key to action mappings.
	UPROPERTY(config, BlueprintReadOnly, EditAnywhere, Category = "EnumMappings", meta = (DisplayName="Mappings with Enum"))
	FNEnhancedEnumMapping NMappingArray;

public:
	UNInputMappingContext();
	UFUNCTION(BlueprintCallable, Category = "Mapping")
	FEnhancedActionKeyMapping& NMapKey(const UInputAction* InAction, FKey ToKey, TEnumAsByte<ENAbilityAction> Enum);
	UFUNCTION(BlueprintCallable, Category = "Mapping")
	void NUnmapKey(const UInputAction* Action, FKey Key, TEnumAsByte<ENAbilityAction> Enum);
	UFUNCTION(BlueprintCallable, Category = "Mapping")
	void NUnmapAllKeysFromAction(const UInputAction* Action);
	UFUNCTION(BlueprintCallable, Category = "Mapping")
	void NUnmapAll();
	
	friend class FInputContextDetails;
	friend class FActionMappingsNodeBuilderEx;

public:	
	#if WITH_EDITOR
		virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	#endif
	/**
	* Mapping accessors.
	* Note: Use UEnhancedInputLibrary::RequestRebuildControlMappingsForContext to invoke changes made to an FNActionKeyMapping
	*/
	const FNEnhancedEnumMapping& GetNMappings() const { return NMappingArray; }
	FEnhancedActionKeyMapping& GetNMapping(TArray<FNEnhancedEnumMapping>::SizeType Index) { return NMappingArray.Mappings[Index]; }

};
