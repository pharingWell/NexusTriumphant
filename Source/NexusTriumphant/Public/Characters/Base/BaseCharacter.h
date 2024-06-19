// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Collision/EntityCollisionComponent.h"
#include "Abilities/AbilitySet.h"
#include "Characters/Attributes/StandardAttributeSet.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "AbilitySystemComponent.h"
#include "BaseCharacter.generated.h"

class UEntityCollisionComponent;
class UInputMappingContext;
class UAbilitySystemComponent;
class UStandardAttributeSet;
class UGameplayAbility;
class UAbilitySet;
struct FGameplayAbilitySpecHandle;

// bitmaskable
UENUM(meta=(Bitflags))
enum EBaseCharacterVisualizeMode
{
	Disabled    = 0b0    UMETA(DisplayName="Disabled"),
	Collision2D = 0b1    UMETA(DisplayName="Collision2D"),
	Other       = 0b10   UMETA(DisplayName="Other"),
	
};

ENUM_CLASS_FLAGS(EBaseCharacterVisualizeMode);
ENUM_RANGE_BY_FIRST_AND_LAST(EBaseCharacterVisualizeMode, EBaseCharacterVisualizeMode::Disabled, EBaseCharacterVisualizeMode::Other);

static EBaseCharacterVisualizeMode DebugVisualizeMode {EBaseCharacterVisualizeMode::Disabled};

UCLASS(Blueprintable)
class ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()


public:
	ABaseCharacter(const FObjectInitializer&);

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty > & OutLifetimeProps) const override;

	
	UFUNCTION(BlueprintCallable)
	void SetupInitialAbilitiesAndEffects();

	UFUNCTION(Exec, Category="Commands")
	static void DebugToggleVisualizations(const FString modeString);

	/** Returns TopDownCameraComponent sub-object **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom sub-object **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UEntityCollisionComponent* CollisionComponent;

	/** MappingContext for player input. **/
	UPROPERTY(VisibleAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	/** Ability System Component, Attributes, Effects and Abilities for setup **/

	UPROPERTY(VisibleAnywhere, Category="Abilities")
	UAbilitySystemComponent* AbilitySystemComponent{nullptr};

	UPROPERTY(Replicated, VisibleAnywhere, Category="Abilities")
	UStandardAttributeSet* StandardAttributes{nullptr};

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	UAbilitySet* InitialAbilitySet{nullptr};

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;

	void OnHealthAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};
