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

// the amount of items in ENGameplayAbility
#define ENGA_ENUM_ITEMS 10

template <class T> struct FNActionBinding
{
	FNActionBinding(const UInputAction* const InputAction,
	                const FEnhancedInputActionHandlerSignature::TMethodPtr<T> InputStartedFunc,
	                const FEnhancedInputActionHandlerSignature::TMethodPtr<T> InputTriggeredFunc,
	                const FEnhancedInputActionHandlerSignature::TMethodPtr<T> InputReleasedFunc)
		: InputAction(InputAction),
		  InputStartedFunc(InputStartedFunc),
		  InputTriggeredFunc(InputTriggeredFunc),
		  InputReleasedFunc(InputReleasedFunc)
	{
	}
	const UInputAction* InputAction;
	const FEnhancedInputActionHandlerSignature::TMethodPtr<T> InputStartedFunc;
	const FEnhancedInputActionHandlerSignature::TMethodPtr<T> InputTriggeredFunc;
	const FEnhancedInputActionHandlerSignature::TMethodPtr<T> InputReleasedFunc;
};

static FGameplayAbilitySpecHandle Blank = FGameplayAbilitySpecHandle();
static TArray<FGameplayAbilitySpecHandle> EmptyHandleArray = {};

USTRUCT()
struct FNActionContainer
{
	GENERATED_BODY()
public:
	UENUM()
	enum ENActionContainerValidity
	{
		INVALID					=0b0,
		ABILITIES				=0b1,
		BINDINGS				=0b10,
	};
private:
	UPROPERTY()
	UObject* OwningObject;
	TMap<ENGameplayAbility, TSubclassOf<UNGameplayAbility>> AbilityMap;
	template <class T>
		TMap<ENGameplayAbility, FNActionBinding<T>> BindingMap;
	TMap<ENGameplayAbility, FGameplayAbilitySpecHandle> GASpecHandles;
	// while initially this was a bitmask, it proved overkill
	bool bValidityAbilities = false;
	bool bValidityBindings = false;

public:
	FNActionContainer(UObject* OwningObject);
	
	bool ContainerIsValid(ENActionContainerValidity BitMask) const;
	void SetActions(TMap<ENGameplayAbility, TSubclassOf<UNGameplayAbility>> ActionMap);
	void SetAction(ENGameplayAbility EnumKey, const TSubclassOf<UNGameplayAbility>& AbilityClass);
	template <class T>
		void SetBindings(TMap<ENGameplayAbility, FNActionBinding<T>> BindingMap);
	template <class T>
		void GiveInputs(UInputComponent* InputComponent);
	template <class T>
		void SetBinding(ENGameplayAbility EnumKey, FNActionBinding<T> Binding);
	void GiveAbilities(UAbilitySystemComponent* ASC);
};




