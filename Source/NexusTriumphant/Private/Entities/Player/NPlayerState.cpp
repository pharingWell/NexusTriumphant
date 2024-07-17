// Fill out your copyright notice in the Description page of Project Settings.
#include "Entities/Player/NPlayerState.h"
#include "AbilitySystem/Abilities/NAbilityHelpers.h"
#include "Entities/Player/NPlayerCharacter.h"
#include "Net/UnrealNetwork.h"



ANPlayerState::ANPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Ability system items
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	
	InitialAbilitySet = CreateDefaultSubobject<UNAbilitySet>(TEXT("InitialAbilitySet"));
	StandardAttributes = CreateDefaultSubobject<UNBaseAttributeSet>(TEXT("StandardAttributeSet"));

	if (AbilitySystemComponent)
	{
		//AbilitySystemComponent->InitAbilityActorInfo(this, PLAYER);
	}
}

void ANPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANPlayerState, StandardAttributes);
}


void ANPlayerState::SetupInitialAbilitiesAndEffects()
{
	if (IsValid(AbilitySystemComponent) == false || IsValid(StandardAttributes) == false)
	{
		return;
	}

	if (IsValid(InitialAbilitySet))
	{
		InitiallyGrantedAbilitySpecHandles.Append(
			InitialAbilitySet->UNAbilitySet::GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
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


	//AbilitySystemComponent->ApplyGameplayEffectToSelf(CreateDefaultSubobject<UGameplayEffect>())

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UNBaseAttributeSet::GetHealthAttribute())
						  .AddUObject(this, &ThisClass::OnHealthAttributeChanged);
}

void ANPlayerState::OnHealthAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Health updated"));
	if (FMath::IsNearlyZero(OnAttributeChangeData.NewValue) && OnAttributeChangeData.OldValue > 0)
	{
		
	}
}