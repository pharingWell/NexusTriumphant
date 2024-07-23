#pragma once

// the amount of items in ENGameplayAbility
#define ENGA_ENUM_ITEMS 10
#define ENGA_ADDITIONAL_ABILITIES 4
#include "AbilitySystemComponent.h"
#include "NGameplayAbility.h"
#include "Abilities/GameplayAbility.h"

DECLARE_LOG_CATEGORY_EXTERN(LogActionSystem, Verbose, All);

class UNGameplayAbility;
class UAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;

UENUM()
enum ENGameplayAbility
{
	ATTACK,
	ABILITY1,
	ABILITY2,
	ABILITY3,
	ULTIMATE,
	TRAIT,
	ADDT1,
	ADDT2,
	ADDT3,
	ADDT4,
};

USTRUCT()
struct FNActionContainer
{
	GENERATED_BODY()
	explicit FNActionContainer(const TArray<FGameplayAbilitySpecHandle>& SpecHandles)
		: SpecHandles(SpecHandles){}

private:
	const TArray<FGameplayAbilitySpecHandle>& SpecHandles;
};


static FGameplayAbilitySpecHandle Blank = FGameplayAbilitySpecHandle();
static TArray<FGameplayAbilitySpecHandle> InvalidArray = {};

USTRUCT()
struct FNChampionActions
{
	GENERATED_BODY()
private:
	TMap<ENGameplayAbility, TSubclassOf<UNGameplayAbility>> AbilityMap;
	TArray<FGameplayAbilitySpecHandle> GASpecHandles;
public:
	void SetActions(TMap<ENGameplayAbility, TSubclassOf<UNGameplayAbility>> ActionMap);
	void SetAction(ENGameplayAbility EnumKey, const TSubclassOf<UNGameplayAbility>& AbilityClass);
	void GiveAbilities(UObject* CallingObject, UAbilitySystemComponent* ASC);
	FNActionContainer GetSpecHandles(OUT bool& bSucceededOut);
};


