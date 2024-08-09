// Fill out your copyright notice in the Description page of Project Settings.
#include "Entities/Player/NPlayerState.h"
#include "AbilitySystem/NActionHelper.h"
#include "AbilitySystem/Abilities/NAbilityHelpers.h"
#include "Entities/Player/NPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "NexusTriumphant/NexusTriumphant.h"


ANPlayerState::ANPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Ability system items
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	ChampionDataAsset = CreateDefaultSubobject<UNChampionDef>("Champion");
	//InitialAbilitySet = CreateDefaultSubobject<UNAbilitySet>(TEXT("InitialAbilitySet"));
	//StandardAttributes = CreateDefaultSubobject<UNBaseAttributeSet>(TEXT("StandardAttributeSet"));
	
	if (AbilitySystemComponent)
	{
		//AbilitySystemComponent->InitAbilityActorInfo(this, PLAYER);
	}
}

void ANPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ANPlayerState, StandardAttributes);
}

void ANPlayerState::SetupNPS(TObjectPtr<UNPlayerSystem> InNPS)
{
	NPS = InNPS;
}

void ANPlayerState::BeginPlay()
{
	Super::BeginPlay();

	//SetupInitialAbilitiesAndEffects();
}
