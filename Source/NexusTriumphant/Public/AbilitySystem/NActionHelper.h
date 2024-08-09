#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogActionSystem, Verbose, All);

UENUM()
enum ENAbilityAction //Nexus Ability Action: What action the play is taking to activate these things
{
	INVALID = -1,

	ATTACK,
	ABILITY1,
	ABILITY2,
	ABILITY3,
	MOVETO,
	ULTIMATE,
	TRAIT,
	ADDT1,
	ADDT2,
};




