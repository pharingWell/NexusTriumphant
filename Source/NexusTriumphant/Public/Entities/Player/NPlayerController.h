// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystem/NExecuteActionComponent.h"
#include "NPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;



UCLASS()
class ANPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** BASE ACTIONS */
	
	/** Destination MoveTo Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveToAction;

	/** Destination Attack Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** BASE ABILITIES */
	
	/** Activate Ability1 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ActivateAbility1;
	
	/** Activate Ability2 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ActivateAbility2;

	/** Activate Ability3 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ActivateAbility3;

	/** Activate AbilityUltimate Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ActivateAbilityUltimate;

	/** Activate AbilityTrait Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ActivateAbilityTrait;

	/** ADDITIONAL ABILITIES */
	
	/** Activate Additional Ability #1 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ActivateAddtAbility1;

	/** Activate Additional Ability #2 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ActivateAddtAbility2;

	/** Activate Additional Ability #3 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ActivateAddtAbility3;

	/** Activate Additional Ability #4 Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ActivateAddtAbility4;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ExecuteAction", meta = (AllowPrivateAccess = "true"))
	class UNExecuteActionComponent* ExecuteActionComponent;
private:
	FVector CachedDestination;
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
	
public:
	ANPlayerController(const FObjectInitializer& ObjectInitializer);
	// Called to bind functionality to input
	
	
protected:
	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(APawn* P) override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();
};


