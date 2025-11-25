// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/NPlayerState.h"
#include "AbilitySystem/NActionHelper.h"
#include "AbilitySystem/Abilities/NAbilityHelpers.h"
#include "Player/NPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "NexusTriumphant/NexusTriumphant.h"


ANPlayerState::ANPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Ability system items
	NAbilitySystemComponent = CreateDefaultSubobject<UNAbilitySystemComponent>(TEXT("NAbilitySystemComponent"));
	ChampionDataAsset = CreateDefaultSubobject<UNChampionDef>(TEXT("Champion Definition"));
	//InitialAbilitySet = CreateDefaultSubobject<UNAbilitySet>(TEXT("InitialAbilitySet"));
	//StandardAttributes = CreateDefaultSubobject<UNBaseAttributeSet>(TEXT("StandardAttributeSet"));
}

void ANPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ANPlayerState, ChampionDataAsset, COND_InitialOnly);
}

void ANPlayerState::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;
	NAbilitySystemComponent = NewObject<UNAbilitySystemComponent>(this, UNAbilitySystemComponent::StaticClass());
	NAbilitySystemComponent->RegisterComponent();
	//SetupInitialAbilitiesAndEffects();
}
