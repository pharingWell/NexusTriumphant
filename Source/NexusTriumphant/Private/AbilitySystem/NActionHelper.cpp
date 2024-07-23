#include "S:\Unreal\Engine\Intermediate\Build\Win64\x64\UnrealEditorGPF\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.Cpp20.h"
#include "AbilitySystem/NActionHelper.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"

DEFINE_LOG_CATEGORY(LogActionSystem);

template <class T> 
static void AddInputToComponent (UObject* CallingObject, UInputAction* InputAction,
	FEnhancedInputActionHandlerSignature::TMethodPtr<T> InputStartedFunc,
	FEnhancedInputActionHandlerSignature::TMethodPtr<T> InputTriggeredFunc,
	FEnhancedInputActionHandlerSignature::TMethodPtr<T> InputReleasedFunc,
	UInputComponent* InputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		
		// Setup mouse input events
		
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, CallingObject, &InputStartedFunc);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, CallingObject, &InputTriggeredFunc);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, CallingObject, &InputReleasedFunc);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, CallingObject, &InputReleasedFunc);
	}
	else
	{
		UE_LOG(LogActionSystem, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void FNChampionActions::SetActions(TMap<ENGameplayAbility, TSubclassOf<UNGameplayAbility>> ActionMap)
{
	for (auto Pair : ActionMap)
	{
		SetAction(Pair.Key, Pair.Value);
	}
}

void FNChampionActions::SetAction(ENGameplayAbility EnumKey, const TSubclassOf<UNGameplayAbility>& AbilityClass)
{
	AbilityMap.Add(EnumKey, AbilityClass);
}

void FNChampionActions::GiveAbilities(UObject* CallingObject, UAbilitySystemComponent* ASC) 
{
	if (!IsValid(ASC))
	{
		return;
	}

	GASpecHandles.Empty();
	for(int i = 0; i < ENGA_ENUM_ITEMS; i++){ GASpecHandles.Add(Blank); }
	for (auto Pair : AbilityMap)
	{
		if(IsValid(Pair.Value))
		{
			GASpecHandles[Pair.Key] = ASC->GiveAbility(FGameplayAbilitySpec(Pair.Value, 1, Pair.Key, CallingObject));
		}else
		{
			
		}
	}
	
}

const TArray<FGameplayAbilitySpecHandle>& FNChampionActions::GetSpecHandles(OUT bool& bSucceededOut)
{
	if(GASpecHandles.IsEmpty() || GASpecHandles.Num() < ENGA_ENUM_ITEMS)
	{
		bSucceededOut = false;
		return InvalidArray;
	}

	bSucceededOut = true;
	return GASpecHandles;
	
}