// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/NBaseAttributeSet.h"

#include "GameplayEffectExtension.h"


void UNBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetHealthMax());
	}
}

void UNBaseAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float DamageDone = GetDamage();
		SetDamage(0.0f);

		if(DamageDone > 0.0f)
		{
			const float NewHealthValue = GetHealth() - DamageDone;
			SetHealth(FMath::Clamp(NewHealthValue, 0.0f, GetHealthMax()));
		}
	}
	
}

void UNBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealthValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNBaseAttributeSet, Health, OldHealthValue);
}

void UNBaseAttributeSet::OnRep_HealthMax(const FGameplayAttributeData& OldHealthMaxValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNBaseAttributeSet, HealthMax, OldHealthMaxValue);
}

void UNBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

