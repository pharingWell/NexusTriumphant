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
	ChampionDataAsset = CreateDefaultSubobject<UNDA_Champion>("Champion");
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


void ANPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if(ChampionDataAsset)
	{

		// creates a gameplay spec for each ability assigned via the champion asset
		TMap<ENGameplayAbility, TSubclassOf<UNGameplayAbility>> AbilityMap{
			{ENGameplayAbility::ATTACK, ChampionDataAsset->AttackClass},
			{ENGameplayAbility::ABILITY1, ChampionDataAsset->Ability1Class},
			{ENGameplayAbility::ABILITY2, ChampionDataAsset->Ability2Class},
			{ENGameplayAbility::ABILITY3, ChampionDataAsset->Ability3Class},
			{ENGameplayAbility::ULTIMATE, ChampionDataAsset->UltimateClass},
			{ENGameplayAbility::TRAIT, ChampionDataAsset->TraitClass},
		};
		ChampionActionsStruct.SetActions(AbilityMap);
		ChampionActionsStruct.GiveAbilities(GetAbilitySystemComponent());
		const FNActionContainer FNActions = ChampionActionsStruct.GetActionContainer();
		if(FNActionContainer::IsValid(FNActions))
		{
			SpecHandles = FNActions;
		}else
		{
			UE_LOG_ABILITY_CAUTION("NDA_Champion spec handles failed to be captured", this);
		}
	}else
	{
		UE_LOG_ABILITY_CAUTION("NDA_Champion not found at begin play", this);
	}

	
	//SetupInitialAbilitiesAndEffects();
}

/*
void ANPlayerState::SetupInitialAbilitiesAndEffects()
{
	if (IsValid(AbilitySystemComponent) == false || IsValid(StandardAttributes) == false)
	{
		UE_LOG_ABILITY_CAUTION("AbilitySystemComponent or StandardAttributes is invalid", this);
		return;
	}

	if (IsValid(InitialAbilitySet))
	{
		InitiallyGrantedAbilitySpecHandles.Append(
			InitialAbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
	}
	else
	{
		UE_LOG_ABILITY_CAUTION("Initial Ability Set is invalid", this);
	}

	if (IsValid(InitialGameplayEffect))
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(
			InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(),
			0.f,
			AbilitySystemComponent->MakeEffectContext()
		);
	}
	else
	{
		UE_LOG_ABILITY_CAUTION("Initial Ability Effect is invalid", this);
	}


	//AbilitySystemComponent->ApplyGameplayEffectToSelf(CreateDefaultSubobject<UGameplayEffect>());

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UNBaseAttributeSet::GetHealthAttribute())
						  .AddUObject(this, &ThisClass::OnHealthAttributeChanged);
	
}

void ANPlayerState::OnHealthAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Health updated"));
	if (FMath::IsNearlyZero(OnAttributeChangeData.NewValue) && OnAttributeChangeData.OldValue > 0)
	{
		
	}
}*/