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
	//SetupInitialAbilitiesAndEffects();
}


void ANPlayerState::Setup()
{
	NPlayerController = Cast<ANPlayerController>(this->GetPlayerController());
	if(!NPlayerController)
	{
		UE_LOG(LogNAbilitySystem, Error, TEXT("[NPlayerState] NPlayerController invalid"));
		return;
	}6
	TMap<ENAbilityAction, FString> Names {};
	auto AbilityMap = ChampionDataAsset->GetUpdatedAbilityMap();
	if(NPlayerController->HasAuthority())
	{
		CurrentAbilityActions.SetNum(AbilityMap.Num());
		UE_LOG(LogNAbilitySystem, Warning, TEXT("%i:::::"), AbilityMap.Num())
		for (const auto AbilityPair : AbilityMap)
		{
			const ENAbilityAction &AbilityAction = AbilityPair.Key;
			if(AbilityAction == ENAbilityAction::ENQUEUE)
			{
				continue;
			}
			const TSubclassOf<UGameplayAbility> &GameplayAbility = AbilityPair.Value;

			FGameplayAbilitySpecHandle Handle = NPlayerController->GetAbilitySystemComponent()->GiveAbility(
			FGameplayAbilitySpec(GameplayAbility, 1 /* abil level */, static_cast<uint8>(AbilityAction), this));
			BaseAbilityActions.Add(AbilityAction, Handle);
			CurrentAbilityActions[static_cast<uint8>(AbilityAction)] = Handle;
			Names.Add(AbilityAction, GameplayAbility->GetDescription());
		}

		FString String = "";
		for (auto Element : BaseAbilityActions)
		{
			String += FString::Printf(TEXT("[%d, %s, %s]"), Element.Key, *Element.Value.ToString(), *Names[Element.Key]);
		}
		UE_LOG(LogActionSystem, Display, TEXT("[NPlayerActionComponent] CurrentAbilityActions: {%s}"), *String);
	}
	bSetup = true;
}

/** Restores the Base Ability Action to the Current Ability Action slot */
void ANPlayerState::RevertAbilityAction(const ENAbilityAction Action)
{
	if(!bSetup) return;
	if(!CurrentAbilityActions[static_cast<uint8>(Action)].IsValid())
	{
		return;
	}
	if(!BaseAbilityActions.Contains(Action))
	{
		CurrentAbilityActions[static_cast<uint8>(Action)] = FGameplayAbilitySpecHandle();
		return;
	}
	CurrentAbilityActions[static_cast<uint8>(Action)] = BaseAbilityActions[Action];
}

FGameplayAbilitySpecHandle ANPlayerState::GetHandle(const ENAbilityAction Action, const bool GetBase /* false */)
{
	if(!bSetup)
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerActionComponent] GetHandle called before setup. Returning blank handle"))
		return FGameplayAbilitySpecHandle();
	}
	if(GetBase)
	{
		if(BaseAbilityActions.Contains(Action))
		{
			checkf(BaseAbilityActions[Action].IsValid(),
				TEXT("[NPlayerState] BaseAbilityActions assumption of validity failed, %s is invalid"),
				*BaseAbilityActions[Action].ToString()
			);
			return BaseAbilityActions[Action];
		}
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("[NPlayerState] Blank handle for GetBase"))
		return FGameplayAbilitySpecHandle();
	}

	if(CurrentAbilityActions[static_cast<uint8>(Action)].IsValid())
	{
		return CurrentAbilityActions[static_cast<uint8>(Action)];
	} 
	UE_LOG(LogNAbilitySystem, Error, TEXT("[NPlayerState] CurrentAbilityActions assumption of validity failed, %s is invalid"),
				*CurrentAbilityActions[static_cast<uint8>(Action)].ToString())
	return FGameplayAbilitySpecHandle();
}