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

void ANPlayerState::RevertAbilityAction(ENAbilityAction Action)
{
	if(!CurrentAbilityActions.Contains(Action))
	{
		return;
	}
	if(!BaseAbilityActions.Contains(Action))
	{
		CurrentAbilityActions.Remove(Action);
		return;
	}
	CurrentAbilityActions[Action] = BaseAbilityActions[Action];

}

bool ANPlayerState::RunAction(const ENAbilityAction Action)
{
	return GetAbilitySystemComponent()->TryActivateAbility(GetHandle(Action));
}

FGameplayAbilitySpecHandle& ANPlayerState::GetHandle(const ENAbilityAction Action, const bool GetBase)
{
	if(GetBase)
	{
		if(BaseAbilityActions.Contains(Action))
		{
			checkf(!BaseAbilityActions[Action].IsValid(),
				TEXT("[NPlayerState] BaseAA assumption of validity failed, %s is invalid"),
				*BaseAbilityActions[Action].ToString()
			);
			return BaseAbilityActions[Action];
		}
		return BlankHandle;
	}

	if(CurrentAbilityActions.Contains(Action))
	{
		checkf(!CurrentAbilityActions[Action].IsValid(),
				TEXT("[NPlayerState] CurrentAA assumption of validity failed, %s is invalid"),
				*CurrentAbilityActions[Action].ToString()
			);
		return CurrentAbilityActions[Action];
	}
	
	return BlankHandle;
}


void ANPlayerState::BeginPlay()
{
	Super::BeginPlay();

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