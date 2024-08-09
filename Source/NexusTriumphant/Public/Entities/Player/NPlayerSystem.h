#pragma once
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/NActionHelper.h"
#include "Entities/Player/NPlayerInputDef.h"
#include "NPlayerSystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerSystem, Verbose, All);

class ANPlayerCharacter;
class ANPlayerController;
class ANPlayerState;
class UNPlayerActionComponent;

USTRUCT()
struct NEXUSTRIUMPHANT_API FNPlayerSystem
{
	GENERATED_BODY()
public:
	TObjectPtr<UNPlayerActionComponent> NPlayerActionComponent;
	TObjectPtr<ANPlayerState> NPlayerState;
	TObjectPtr<ANPlayerController> NPlayerController;
	TObjectPtr<ANPlayerCharacter> NPlayerCharacter;
	TObjectPtr<UAbilitySystemComponent> ASC;
	TSharedPtr<FGameplayAbilityActorInfo> AbilityActorInfo;

private:
	bool bValid = false;
	
public:
	void Setup(const TObjectPtr<UNPlayerActionComponent>& ActionComponent, const TObjectPtr<ANPlayerState>& PlayerState,
	           const TObjectPtr<ANPlayerController>& Controller, const TObjectPtr<ANPlayerCharacter>& Character,
	           const TObjectPtr<UAbilitySystemComponent>& InASC);
	FORCEINLINE bool Valid() const { return bValid; }
};

UINTERFACE()
class NEXUSTRIUMPHANT_API UNPlayerSystemInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class NEXUSTRIUMPHANT_API INPlayerSystemInterface
{
	GENERATED_IINTERFACE_BODY()
	virtual void SetupNPS(FNPlayerSystem* InNPS) = 0;
	
};