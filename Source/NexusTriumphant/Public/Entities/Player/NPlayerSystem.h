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

UCLASS()
class UNPlayerSystem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UNPlayerActionComponent> NPlayerActionComponent;
	UPROPERTY()
	TObjectPtr<ANPlayerState> NPlayerState;
	UPROPERTY()
	TObjectPtr<ANPlayerController> NPlayerController;
	UPROPERTY()
	TObjectPtr<ANPlayerCharacter> NPlayerCharacter;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	FGameplayAbilityActorInfo AbilityActorInfo;

private:
	bool bValid = false;
	
public:
	UNPlayerSystem::UNPlayerSystem(const FObjectInitializer&);
	void Setup(TObjectPtr<UNPlayerActionComponent> ActionComponent, TObjectPtr<APlayerState> PlayerState,
	               TObjectPtr<ANPlayerController> Controller, TObjectPtr<APawn> Pawn,
	               TObjectPtr<UAbilitySystemComponent> ASC);
	FORCEINLINE bool IsStructValid() const { return bValid; }
};

UINTERFACE()
class UNPlayerSystemInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class INPlayerSystemInterface
{
	GENERATED_IINTERFACE_BODY()
	virtual void SetupNPS(TObjectPtr<UNPlayerSystem> InNPS) = 0;
	
};