// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Player/NPlayerInputDef.h"


UNPlayerInputDef::UNPlayerInputDef(const FObjectInitializer& ObjectInitializer)
{
	AttackInput = CreateDefaultSubobject<UInputAction>("Attack Input Action");
	Ability1Input = CreateDefaultSubobject<UInputAction>("First Ability Input Action");
	Ability2Input = CreateDefaultSubobject<UInputAction>("Second Ability Input Action");
	Ability3Input = CreateDefaultSubobject<UInputAction>("Third Ability Input Action");
	UltimateInput = CreateDefaultSubobject<UInputAction>("Ultimate Ability Input Action");
	TraitInput = CreateDefaultSubobject<UInputAction>("Trait Ability Input Action");
	AdditionalAbility1Input = CreateDefaultSubobject<UInputAction>("First Additional Ability Input Action");
	AdditionalAbility2Input = CreateDefaultSubobject<UInputAction>("Second Additional Ability Input Action");
	
	BindingMap=  {
		{ENAbilityAction::ATTACK, AttackInput},
		{ENAbilityAction::ABILITY1, Ability1Input},
		{ENAbilityAction::ABILITY2, Ability2Input},
		{ENAbilityAction::ABILITY3, Ability3Input},
		{ENAbilityAction::ULTIMATE, UltimateInput},
		{ENAbilityAction::TRAIT, TraitInput},
		{ENAbilityAction::ADDT1, AdditionalAbility1Input},
		{ENAbilityAction::ADDT2, AdditionalAbility2Input}
	};
}
