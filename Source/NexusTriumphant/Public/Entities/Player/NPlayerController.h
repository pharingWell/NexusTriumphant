// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/NActionHelper.h"
#include "Entities/Player/NPlayerState.h"
#include "Entities/Player/NPlayerCharacter.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "NPlayerController.generated.h"

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
	FGameplayAbilitySpecHandle CurrentActionSpecHandle;
	FGameplayAbilityActorInfo AbilityActorInfo;
	bool bExecutingQueue;
	TQueue<ENAbilityAction> Queue;
private:
	FVector CachedDestination;
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
	UPROPERTY()
	ANPlayerState* NPlayerState;
	UPROPERTY()
	ANPlayerCharacter* NPlayerCharacter;
	UPROPERTY()
	UAbilitySystemComponent* ASCRef;
	bool bASCRefValid;
	
public:
	ANPlayerController(const FObjectInitializer& ObjectInitializer);
	virtual void OnConstruction(const FTransform & Transform) override;
	// Called to bind functionality to input
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void ExecuteAction(const ENAbilityAction Action);
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void CancelCurrentAction();
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void EnqueueAction(const ENAbilityAction Action);
	
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	void ClearQueue();
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
	void UpdateASCRef()
	{
		ASCRef = GetAbilitySystemComponent();
		bASCRefValid = IsValid(ASCRef);
	}
	UFUNCTION()
	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(APawn* P) override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	bool RunAbilityAction(ENAbilityAction Action);
	void ExecuteQueue();
	void ExecuteQueuedAction();
	UFUNCTION()
	void ActionEnded(const FAbilityEndedData& AbilityEndedData);
	UFUNCTION(Blueprintable, Category = "Actions")
	void OnInputStarted(ENAbilityAction InputUsed);
	UFUNCTION(Blueprintable, Category = "Actions")
	void OnInputTriggered(ENAbilityAction InputUsed);
	UFUNCTION(Blueprintable, Category = "Actions")
	void OnInputFinished(ENAbilityAction InputUsed);
};


