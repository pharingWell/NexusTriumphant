#pragma once


#include "NGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

DECLARE_LOG_CATEGORY_EXTERN(LogActionSystem, Verbose, All);

class UNGameplayAbility;
class UAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;

UENUM()
enum ENAbilityAction //Nexus Gameplay Ability Action: What action the play is taking to activate these things
{
	ATTACK,
	ABILITY1,
	ABILITY2,
	ABILITY3,
	ULTIMATE,
	TRAIT,
	ADDT1,
	ADDT2,
};

// the amount of items in ENAbilityAction
#define ENGA_ENUM_ITEMS 10

static TArray<FGameplayAbilitySpecHandle> EmptyHandleArray = {};

USTRUCT()
struct FNActionContainer
{
	GENERATED_BODY()
public:
	// UENUM()
	// enum ENActionContainerValidity
	// {
	// 	INVALID					=0b0,
	// 	ABILITIES				=0b1,
	// 	BINDINGS				=0b10,
	// };
private:
	UPROPERTY()
	UObject* OwningObject;
	TMap<ENAbilityAction, TSubclassOf<UNGameplayAbility>> AbilityMap;
	TMap<ENAbilityAction, FGameplayAbilitySpecHandle> GASpecHandles;
	// while initially this was a bitmask, it proved overkill
	bool bValidityAbilities = false;
	bool bValidityBindings = false;

public:
	FNActionContainer(UObject* OwningObject);
	
	//bool ContainerIsValid(ENActionContainerValidity Validity) const;
	bool ContainerIsValid() const;
	void SetActions(const TMap<ENAbilityAction, TSubclassOf<UNGameplayAbility>>& ActionMap);
	void SetAction(ENAbilityAction EnumKey, const TSubclassOf<UNGameplayAbility>& AbilityClass);
	//These need to be templated because the class of functions that are being bound to is unknown to the struct
	void GiveAbilities(UAbilitySystemComponent* ASC);
};




