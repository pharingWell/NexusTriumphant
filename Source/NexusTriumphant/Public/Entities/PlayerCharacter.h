// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/BaseEntity.h"

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Abilities/AbilitySet.h"
#include "Entities/Attributes/StandardAttributeSet.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Abilities/AbilitySet.h"
#include "Entities/Attributes/StandardAttributeSet.h"
#include "Abilities/AbilityHelpers.h"
#include "PlayerCharacter.generated.h"

class UEntityCollisionComponent;
class UInputMappingContext;
class UAbilitySystemComponent;
class UStandardAttributeSet;
class UGameplayAbility;
class UAbilitySet;
struct FGameplayAbilitySpecHandle;

UCLASS()
class NEXUSTRIUMPHANT_API APlayerCharacter : public ABaseEntity, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:

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

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;	

/** FUNCTIONS */
	
public:
	// Sets default values for this character's properties
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty > & OutLifetimeProps) const override;

	/** Returns TopDownCameraComponent sub-object **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom sub-object **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	
	UFUNCTION(BlueprintCallable)
	void SetupInitialAbilitiesAndEffects();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void OnHealthAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;

private:
	

};
