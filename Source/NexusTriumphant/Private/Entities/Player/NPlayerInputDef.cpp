// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Player/NPlayerInputDef.h"


UNPlayerInputDef::UNPlayerInputDef(const FObjectInitializer& ObjectInitializer): MoveToInput(nullptr),
	AttackInput(nullptr),
	Ability1Input(nullptr),
	Ability2Input(nullptr),
	Ability3Input(nullptr),
	UltimateInput(nullptr),
	TraitInput(nullptr),
	AdditionalAbility1Input(nullptr),
	AdditionalAbility2Input(nullptr)
{
	 MoveToInput = CreateDefaultSubobject<UInputAction>("MoveTo");
	 AttackInput = CreateDefaultSubobject<UInputAction>("Attack");
	 Ability1Input = CreateDefaultSubobject<UInputAction>("Ability 1");
	 Ability2Input = CreateDefaultSubobject<UInputAction>("Ability 2");
	 Ability3Input = CreateDefaultSubobject<UInputAction>("Ability 3");
	 UltimateInput = CreateDefaultSubobject<UInputAction>("Ultimate");
	 TraitInput = CreateDefaultSubobject<UInputAction>("Trait");
	 AdditionalAbility1Input = CreateDefaultSubobject<UInputAction>("Additional 1");
	 AdditionalAbility2Input = CreateDefaultSubobject<UInputAction>("Additional 2");
	 MappingContext = CreateDefaultSubobject<UInputMappingContext>("Mapping Context");

	BindingMap = {
		{ENAbilityAction::MOVETO, MoveToInput},
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
