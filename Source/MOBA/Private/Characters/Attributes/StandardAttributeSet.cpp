// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Attributes/StandardAttributeSet.h"

#include "GameplayEffectExtension.h"

void UStandardAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetHealthMax());
	}
}

void UStandardAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
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

void UStandardAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealthValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStandardAttributeSet, Health, OldHealthValue);
}

void UStandardAttributeSet::OnRep_HealthMax(const FGameplayAttributeData& OldHealthMaxValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStandardAttributeSet, HealthMax, OldHealthMaxValue);
}

void UStandardAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

