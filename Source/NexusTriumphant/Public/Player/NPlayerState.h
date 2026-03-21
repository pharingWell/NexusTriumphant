// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Player/NChampionDef.h"
#include "AbilitySystem/Abilities/NAbilitySet.h"
#include "AbilitySystem/Attributes/NBaseAttributeSet.h"

#include "GameFramework/PlayerState.h"
#include "NPlayerState.generated.h"

class ANPlayerController;
/**
 * 
 */
UCLASS()
class NEXUSTRIUMPHANT_API ANPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	/** PROPERTIES */
protected:
	bool bSetup = false;
private:
	/** Ability System Component, Attributes, Effects and Abilities for setup */
	UPROPERTY(Replicated)
	TObjectPtr<ANPlayerController> NPlayerController;
	UPROPERTY(Replicated, EditDefaultsOnly, Category="Abilities")
	UNChampionDef* ChampionDataAsset;
	UPROPERTY(Replicated, VisibleAnywhere, Category="Abilities")
	TArray<FGameplayAbilitySpecHandle> BaseAbilityActions;
	// index being the enum
	UPROPERTY(Replicated, VisibleAnywhere, Category="Abilities")
	TArray<FGameplayAbilitySpecHandle> CurrentAbilityActions;
	UPROPERTY(Replicated, VisibleAnywhere, Category="Abilities")
	TArray<FString> AbilityNames;
	/** FUNCTIONS */

public:
	ANPlayerState(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;


	UFUNCTION()
	void RevertAbilityAction(ENAbilityAction Action);
	UFUNCTION(BlueprintCallable, Category="Gameplay Ability System")
	FGameplayAbilitySpecHandle GetHandle(ENAbilityAction Action, bool GetBase = false);
	// IAbilitySystemInterface
	UNChampionDef* GetChampionDataAsset() const { return ChampionDataAsset; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category="Gameplay Ability")
	void PrintNames(FString prefix) const;
	UFUNCTION()
	void Setup();
	UFUNCTION()
	bool IsSetup() const { return bSetup; }
	
	UFUNCTION(Server, Reliable)
	void Server_Setup();
};
