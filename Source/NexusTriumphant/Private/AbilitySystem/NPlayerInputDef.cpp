// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NInputSpec.h"


UNPlayerInputDef::UNPlayerInputDef(const FObjectInitializer& ObjectInitializer) :
	BindingMap({
		{ENAbilityAction::ATTACK, AttackInput},
		{ENAbilityAction::ABILITY1, Ability1Input},
		{ENAbilityAction::ABILITY2, Ability2Input},
		{ENAbilityAction::ABILITY3, Ability3Input},
		{ENAbilityAction::ULTIMATE, UltimateInput},
		{ENAbilityAction::TRAIT, TraitInput},
		{ENAbilityAction::ADDT1, AdditionalAbility1Input},
		{ENAbilityAction::ADDT2, AdditionalAbility2Input}
	})
{}
