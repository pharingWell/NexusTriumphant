#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
class ANPlayerController;

DECLARE_LOG_CATEGORY_EXTERN(LogNAbilitySystem, Warning, All);
#define FULLNAME_CSTR(Object) ToCStr(Object->GetFullName())
#define UE_LOG_ABILITY(Verbosity, String, ...) UE_LOG(LogNAbilitySystem, Type::Verbosity, String, ##__VA_ARGS__);

#define UE_LOG_ABILITY_CAUTION(String, Object)                                                                   \
UE_LOG_ABILITY(Display, TEXT("[Ability System] Caution: %s for %s"), ToCStr(String), FULLNAME_CSTR(Object)); \
GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow,                                                     \
FString::Printf(TEXT("[Ability System] Caution: %s for %s"), ToCStr(String), ToCStr(Object->GetName()))  \
);



/*
/** Metadata for a tag-based Gameplay Event, that can activate other abilities or run ability-specific logic #1#
USTRUCT(BlueprintType)
struct GAMEPLAYABILITIES_API FNGameplayEventData
{
	GENERATED_USTRUCT_BODY()

	FNGameplayEventData()
		: Instigator(nullptr)
		, Target(nullptr)
		, OptionalObject(nullptr)
		, OptionalObject2(nullptr)
		, EventMagnitude(0.f)
	{
	}
	
	/** Tag of the event that triggered this #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	FGameplayTag EventTag;

	/** The instigator of the event, the controller #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	TObjectPtr<const AActor> Instigator;

	/** The target of the event, the controller #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	TObjectPtr<const AActor> Target;
	
	/** An optional ability-specific object to be passed though the event #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	TObjectPtr<const UObject> OptionalObject;

	/** A second optional ability-specific object to be passed though the event #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	TObjectPtr<const UObject> OptionalObject2;

	/** Polymorphic context information #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	FGameplayEffectContextHandle ContextHandle;

	/** Tags that the instigator has #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	FGameplayTagContainer InstigatorTags;

	/** Tags that the target has #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	FGameplayTagContainer TargetTags;

	/** The magnitude of the triggering event #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	float EventMagnitude;

	/** The polymorphic target information for the event #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbilityTriggerPayload)
	FGameplayAbilityTargetDataHandle TargetData;
};*/