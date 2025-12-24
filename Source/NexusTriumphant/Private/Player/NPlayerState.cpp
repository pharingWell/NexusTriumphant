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
	DOREPLIFETIME(ANPlayerState, BaseAbilityActions);
	DOREPLIFETIME(ANPlayerState, CurrentAbilityActions);
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
	}
	TArray<FString> Names {};
	auto AbilityMap = ChampionDataAsset->GetUpdatedAbilityMap();
	BaseAbilityActions.SetNum(AbilityMap.Num());
	CurrentAbilityActions.SetNum(AbilityMap.Num());
	Names.SetNum(AbilityMap.Num());
	if(NPlayerController->HasAuthority())
	{
		UE_LOG(LogNAbilitySystem, Warning, TEXT("%i:::::"), AbilityMap.Num())
		for (const auto AbilityPair : AbilityMap)
		{
			const ENAbilityAction &AbilityAction = AbilityPair.Key;
			if(AbilityAction == ENAbilityAction::ENQUEUE)
			{
				continue;
			}
			const TSubclassOf<UGameplayAbility> &GameplayAbility = AbilityPair.Value;
			const uint8 AbilityActionInt = static_cast<uint8>(AbilityAction);
			FGameplayAbilitySpecHandle Handle = NPlayerController->GetAbilitySystemComponent()->GiveAbility(
			FGameplayAbilitySpec(GameplayAbility, 1 /* abil level */, AbilityActionInt, this));
			BaseAbilityActions[AbilityActionInt] = Handle;
			CurrentAbilityActions[AbilityActionInt] = Handle;
			Names[AbilityActionInt] = GameplayAbility->GetDescription();
		}

		FString String = "";
		for (int i = 0; i < BaseAbilityActions.Num(); i++)
		{
			if(!BaseAbilityActions[i].IsValid()) { continue; }
			String += FString::Printf(TEXT("[%d, %s, %s]"), i, *BaseAbilityActions[i].ToString(), *Names[i]);
		}
		UE_LOG(LogActionSystem, Display, TEXT("[NPlayerActionComponent] CurrentAbilityActions: {%s}"), *String);
	}
	bSetup = true;
}

/** Restores the Base Ability Action to the Current Ability Action slot */
void ANPlayerState::RevertAbilityAction(const ENAbilityAction Action)
{
	if(!bSetup) return;
	if(NPlayerController->HasAuthority())
	{
		uint8 ActionAbilityInt = static_cast<uint8>(Action);
		if(!CurrentAbilityActions[ActionAbilityInt].IsValid())
		{
			return;
		}
		if(!BaseAbilityActions[ActionAbilityInt].IsValid())
		{
			CurrentAbilityActions[ActionAbilityInt] = FGameplayAbilitySpecHandle();
			return;
		}
		CurrentAbilityActions[ActionAbilityInt] = BaseAbilityActions[ActionAbilityInt];
	}
}

// GET HANDLE IS KINDA FUCKED. WHY AM I DOING IT THIS WAY?
// 	the client needs 

FGameplayAbilitySpecHandle ANPlayerState::GetHandle(ENAbilityAction Action, const bool GetBase /* false */)
{
	if(!bSetup)
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerActionComponent] GetHandle called before setup. Returning blank handle"))
		return FGameplayAbilitySpecHandle();
	}
	if(NPlayerController->HasAuthority())
	{
		const uint8 ActionAbilityInt = static_cast<uint8>(Action);
		if(GetBase)
		{
			if(BaseAbilityActions[ActionAbilityInt].IsValid())
			{
				return BaseAbilityActions[ActionAbilityInt];
			}
			UE_LOG(LogAbilitySystemComponent, Warning, TEXT("[NPlayerState] Blank handle for GetBase"))
			return FGameplayAbilitySpecHandle();
		}

		if(CurrentAbilityActions[ActionAbilityInt].IsValid())
		{	
			return CurrentAbilityActions[ActionAbilityInt];
		} 
		UE_LOG(LogNAbilitySystem, Error, TEXT("[NPlayerState] CurrentAbilityActions assumption of validity failed, %s is invalid"),
					*CurrentAbilityActions[ActionAbilityInt].ToString())
		return FGameplayAbilitySpecHandle();
	}
	UE_LOG(LogNAbilitySystem, Error, TEXT("[NPlayerState] CurrentAbilityActions returning null to client"))
	return FGameplayAbilitySpecHandle();
}