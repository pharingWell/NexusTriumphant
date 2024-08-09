// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "AbilitySystemInterface.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NPlayerSystem.h"
#include "Engine/LocalPlayer.h"
#include "NPlayerController.generated.h"

class UNPlayerActionComponent;
class UEnhancedInputLocalPlayerSubsystem;
/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;


UCLASS()
class NEXUSTRIUMPHANT_API ANPlayerController : public APlayerController, public IAbilitySystemInterface
                                               //, public INPlayerSystemInterface
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
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UNPlayerInputDef* InputDefinition;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	TEnumAsByte<ENAbilityAction> CurrentAction;

	bool bIsEnqueuing;
	FNPlayerSystem NPS;
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> EILPSubsystem;

	UPROPERTY()
	UNPlayerActionComponent* NPActionComponent;
	UPROPERTY()
	ANPlayerState* NPlayerState;
	UPROPERTY()
	ANPlayerCharacter* NPlayerCharacter;
	
private:
	FVector CachedMoveToDestination;
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
	
	
public:
	explicit ANPlayerController(const FObjectInitializer& ObjectInitializer);
	virtual void OnConstruction(const FTransform & Transform) override;
	// Called to bind functionality to input
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	//virtual void SetupNPS(FNPlayerSystem* NPS) override;
protected:
	UFUNCTION()
	virtual void SetupInputComponent() override;
	UFUNCTION()
	virtual void InitPlayerState() override;
	void CompleteSetup();
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(APawn* P) override;
	
	UFUNCTION(Blueprintable, Category = "Actions")
	void EnqueueStarted();
	UFUNCTION(Blueprintable, Category = "Actions")
	void EnqueueEnded();
	UFUNCTION(Blueprintable, Category = "Actions")
	void OnInputStarted(const TEnumAsByte<ENAbilityAction> InputUsed);
	UFUNCTION(Blueprintable, Category = "Actions")
	void OnInputTriggered(const TEnumAsByte<ENAbilityAction> InputUsed);
	UFUNCTION(Blueprintable, Category = "Actions")
	void OnInputFinished(const TEnumAsByte<ENAbilityAction> InputUsed);
};


