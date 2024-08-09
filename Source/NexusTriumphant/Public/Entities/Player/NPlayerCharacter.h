// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/NexusEntity.h"

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "AbilitySystem/Abilities/NAbilitySet.h"
#include "AbilitySystem/Attributes/NBaseAttributeSet.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "NPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/Abilities/NAbilitySet.h"
#include "AbilitySystem/Attributes/NBaseAttributeSet.h"
#include "AbilitySystem/Abilities/NAbilityHelpers.h"
#include "NPlayerCharacter.generated.h"

class UNExecuteActionComponent;
class UNCollisionComponent;
class UInputMappingContext;



UCLASS()
class NEXUSTRIUMPHANT_API ANPlayerCharacter : public ANexusEntity, public IAbilitySystemInterface,
											  public INPlayerSystemInterface
{
	GENERATED_BODY()


	/** PROPERTIES */
public:
protected:
	FNPlayerSystem* NPS;
	/** MappingContext for player input. */
	UPROPERTY(VisibleAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	/**
	 * Begins as null
	 */
	ANPlayerState* PlayerState;
	
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
	ANPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	/** Returns TopDownCameraComponent sub-object **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom sub-object **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		if(IsValid(PlayerState))
		{
			return PlayerState->GetAbilitySystemComponent();
		}
		
		return nullptr;
	}

	virtual void SetupNPS(FNPlayerSystem* InNPS) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState) override;
private:



};
