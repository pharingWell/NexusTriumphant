#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogActionSystem, Verbose, All);

UENUM()
enum ENAbilityAction //Nexus Ability Action: What action the play is taking to activate these things
{
	INVALID = -1,
	
	MOVETO,
	ATTACK,
	ABILITY1,
	ABILITY2,
	ABILITY3,
	ULTIMATE,
	TRAIT,
	ADDT1,
	ADDT2,
};




