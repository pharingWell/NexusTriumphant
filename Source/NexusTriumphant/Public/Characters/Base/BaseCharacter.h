// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "UObject/ObjectMacros.h"
#include "InputMappingContext.h"
#include "Collision/EntityCollisionComponent.h"
#include "Abilities/AbilitySet.h"
#include "Characters/Attributes/StandardAttributeSet.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UEntityCollisionComponent;
class UInputMappingContext;
class UAbilitySystemComponent;
class UStandardAttributeSet;
class UGameplayAbility;
class UAbilitySet;
struct FGameplayAbilitySpecHandle;

UCLASS(Blueprintable)
class ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer&);

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	void SetupInitialAbilitiesAndEffects();
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
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
	UAbilitySystemComponent* AbilitySystemComponent { nullptr };
	
	UPROPERTY(VisibleAnywhere, Category="Abilities")
	UStandardAttributeSet* StandardAttributes { nullptr };

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	UAbilitySet* InitialAbilitySet { nullptr };

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

