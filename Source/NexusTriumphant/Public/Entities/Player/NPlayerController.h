// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "EnhancedInputComponent.h"
#include "NPlayerActionComponent.h"
#include "AbilitySystem/NActionHelper.h"
#include "Entities/Player/NPlayerInputDef.h"
#include "Entities/Player/NPlayerState.h"
#include "Entities/Player/NPlayerCharacter.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "NPlayerController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;



UCLASS()
class ANPlayerController : public APlayerController, public IAbilitySystemInterface
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
	FGameplayAbilityActorInfo AbilityActorInfo;

	bool bIsEnqueuing;
	
	UPROPERTY()
	UNPlayerActionComponent* PlayerActionComponent;
	UPROPERTY()
	ANPlayerState* NPlayerState;
	UPROPERTY()
	ANPlayerCharacter* NPlayerCharacter;
	UPROPERTY()
	UAbilitySystemComponent* ASCRef;
	bool bASCRefValid;
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* EILPSubsystem;
	
	
private:
	FVector CachedDestination;
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
	
	
public:
	explicit ANPlayerController(const FObjectInitializer& ObjectInitializer);
	virtual void OnConstruction(const FTransform & Transform) override;
	// Called to bind functionality to input
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		if(IsValid(NPlayerState))
			return NPlayerState->GetAbilitySystemComponent();
		if(IsValid(NPlayerCharacter))
		{
			return NPlayerCharacter->GetAbilitySystemComponent();
		}
		// if(GetPawn()->Implements<IAbilitySystemInterface>())
		// {
		// 	return Cast<IAbilitySystemInterface>(GetPawn())->GetAbilitySystemComponent();
		// }
		return nullptr;
	}
protected:
	UFUNCTION()
	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(APawn* P) override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	
	UFUNCTION(Blueprintable, Category = "Actions")
	void EnqueueStarted();
	UFUNCTION(Blueprintable, Category = "Actions")
	void EnqueueEnded();
	UFUNCTION(Blueprintable, Category = "Actions")
	void OnInputStarted(TEnumAsByte<ENAbilityAction> InputUsed);
	UFUNCTION(Blueprintable, Category = "Actions")
	void OnInputTriggered(TEnumAsByte<ENAbilityAction> InputUsed);
	UFUNCTION(Blueprintable, Category = "Actions")
	void OnInputFinished(TEnumAsByte<ENAbilityAction> InputUsed);
};


