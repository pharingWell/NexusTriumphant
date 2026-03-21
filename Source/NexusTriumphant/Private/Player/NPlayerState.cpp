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
	DOREPLIFETIME(ANPlayerState, NPlayerController);
	DOREPLIFETIME(ANPlayerState, BaseAbilityActions);
	DOREPLIFETIME(ANPlayerState, CurrentAbilityActions);
	DOREPLIFETIME(ANPlayerState, AbilityNames)
	PrintNames("RepLifetime");
}

void ANPlayerState::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;
	if (HasAuthority())
	{
		if (!IsValid(ChampionDataAsset))
		{
			UE_LOG(LogNAbilitySystem, Warning, TEXT("[NPlayerState]: Server Champion not accessible")); 
			return;
		}
		auto AbilityMap = ChampionDataAsset->GetAbilityMap();
		int size = 0;
		for (const auto AbilityPair : AbilityMap)
		{
			if (IsValid(AbilityPair.Value))
			{
				size++;
			} 
		}
		UE_LOG(LogNAbilitySystem, Warning, TEXT("[NPlayerState]: Server Champion map size %d (valid %d)"), AbilityMap.Num(), size); 
	}
	//SetupInitialAbilitiesAndEffects();
}

void ANPlayerState::Setup()
{
	NPlayerController = Cast<ANPlayerController>(this->GetPlayerController());
	if(!NPlayerController)
	{
		UE_LOG(LogNAbilitySystem, Error, TEXT("[NPlayerState] Local: NPlayerController invalid"));
		return;
	}
	bSetup = true;
}

void ANPlayerState::PrintNames(FString prefix) const
{
	FString String = "";
	for (int i = 0; i < CurrentAbilityActions.Num(); i++)
	{
		if(!CurrentAbilityActions[i].IsValid()) { continue; }
		String += FString::Printf(TEXT("[%d, %s, %s]"), i, *CurrentAbilityActions[i].ToString(), *AbilityNames[i]);
	}
	FString funcName = "PrintNames";
	if (prefix != "") {
		funcName += "(" + prefix + ")";
	}
	
	UE_LOG(LogActionSystem, Display, TEXT("%hs: [NPlayerState] %s — CurrentAbilityActions: {%s}"), HasAuthority() ? "Server" : "Local", *funcName, *String);
}

void ANPlayerState::Server_Setup_Implementation()
{
	if (!HasAuthority()) { return; }
	NPlayerController = Cast<ANPlayerController>(this->GetPlayerController());
	if(!NPlayerController)
	{
		UE_LOG(LogNAbilitySystem, Error, TEXT("[NPlayerState] NPlayerController invalid"));
		return;
	}
	auto AbilityMap = ChampionDataAsset->GetAbilityMap();
	int LastInvalid = static_cast<uint8>(ENAbilityAction::ENQUEUE);
	int Length = AbilityMap.Num() + LastInvalid;
	
	TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities;
	GameplayAbilities.SetNum(Length);
	for (const auto AbilityPair : AbilityMap)
	{
		const ENAbilityAction &AbilityAction = AbilityPair.Key;
		const TSubclassOf<UGameplayAbility> &GameplayAbility = AbilityPair.Value;
		const uint8 AbilityActionInt = static_cast<uint8>(AbilityAction);
		if(AbilityActionInt <= LastInvalid)
		{
			continue;
		}
		GameplayAbilities[AbilityActionInt] = GameplayAbility;
	}
	
	BaseAbilityActions.SetNum(Length);
	CurrentAbilityActions.SetNum(Length);
	AbilityNames.SetNum(Length);


	for (const auto AbilityPair : AbilityMap)
	{
		const ENAbilityAction &AbilityAction = AbilityPair.Key;
		const TSubclassOf<UGameplayAbility> &GameplayAbility = AbilityPair.Value;
		const uint8 AbilityActionInt = static_cast<uint8>(AbilityAction);
		if(AbilityActionInt <= LastInvalid)
		{
			continue;
		}
		FGameplayAbilitySpecHandle Handle = NPlayerController->GetAbilitySystemComponent()->GiveAbility(
		FGameplayAbilitySpec(GameplayAbility, 1 /* abil level */, AbilityActionInt, this));
		BaseAbilityActions[AbilityActionInt] = Handle;
		CurrentAbilityActions[AbilityActionInt] = Handle;
		AbilityNames[AbilityActionInt] = GameplayAbility->GetDescription();
	}
	bSetup = true;
	PrintNames("ServerSetup");
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
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerState] GetHandle called before setup. Returning blank handle"))
		return FGameplayAbilitySpecHandle();
	}
	PrintNames("GetHandle");
	const uint8 ActionAbilityInt = static_cast<uint8>(Action);
	if(GetBase)
	{
		if (ActionAbilityInt >= BaseAbilityActions.Num())
		{
			UE_LOG(LogAbilitySystemComponent, Warning, TEXT("[NPlayerState] Blank handle for GetBase: Invalid index %d < length %d"), ActionAbilityInt, BaseAbilityActions.Num())
			return FGameplayAbilitySpecHandle();
		}
		if(BaseAbilityActions[ActionAbilityInt].IsValid())
		{
			return BaseAbilityActions[ActionAbilityInt];
		}
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("[NPlayerState] Blank handle for GetBase"))
		return FGameplayAbilitySpecHandle();
	}
	FString str = "CurrentAbilityActions: ";
	for (auto Element : CurrentAbilityActions)
	{
		str += Element.ToString() + ",";
	}
	if (ActionAbilityInt >= CurrentAbilityActions.Num())
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("[NPlayerState] Blank handle for Current: Invalid index %d < length %d"), ActionAbilityInt, CurrentAbilityActions.Num())
		return FGameplayAbilitySpecHandle();
	}
	UE_LOG(LogNAbilitySystem, Display, TEXT("%s"), *str);
	if(CurrentAbilityActions[ActionAbilityInt].IsValid())
	{	
		return CurrentAbilityActions[ActionAbilityInt];
	} 
	UE_LOG(LogNAbilitySystem, Error, TEXT("[NPlayerState] CurrentAbilityActions assumption of validity failed, %s is invalid"),
				*CurrentAbilityActions[ActionAbilityInt].ToString())
	return FGameplayAbilitySpecHandle();
}