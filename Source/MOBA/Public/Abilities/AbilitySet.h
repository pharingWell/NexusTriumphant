#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySet.generated.h"

class UInputAction;

USTRUCT()
struct FAbilityInputInfo
{
	GENERATED_BODY()
 
	UPROPERTY(EditAnywhere, Category = "GameplayInputAbilityInfo")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditAnywhere, Category = "GameplayInputAbilityInfo")
	TObjectPtr<UInputAction> InputAction;
 
	/** It will be generated automatically. */
	UPROPERTY(VisibleAnywhere, Category = "GameplayInputAbilityInfo")
	int32 InputID;

	bool IsValid() const
	{
		return GameplayAbility && InputAction;
	}
 
	bool operator==(const FAbilityInputInfo& Other) const
	{
		return GameplayAbility == Other.GameplayAbility && InputAction == Other.InputAction;
	}

	bool operator!=(const FAbilityInputInfo& Other) const
	{
		return !operator==(Other);
	}

	friend uint32 GetTypeHash(const FAbilityInputInfo& Item)
	{
		return HashCombineFast(GetTypeHash(Item.GameplayAbility), GetTypeHash(Item.InputAction));
	}
};

USTRUCT()
struct FAbilitySetItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	int Input;
	
	friend uint32 GetTypeHash(const FAbilitySetItem& AbilitySetItem)
	{
		return HashCombine(GetTypeHash(AbilitySetItem.GameplayAbility), AbilitySetItem.Input);
	}
};

/**
 * https://ukustra.medium.com/how-to-easily-bind-gameplay-ability-system-to-enhanced-input-b45ece2ca02b
 */
UCLASS()
class MOBA_API UAbilitySet : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AbilitySystem")
	TSet<FAbilityInputInfo> InputAbilities;
 
public:
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSet<FAbilitySetItem> AbilitySetItems;

	const TSet<FAbilityInputInfo>& GetInputAbilities() const;
	TArray<FGameplayAbilitySpecHandle> GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};