#include "Entities/Player/NPlayerSystem.h"
#include "Entities//Player/NPlayerController.h"
#include "Entities/Player/NPlayerActionComponent.h"
#include "Entities/Player/NPlayerCharacter.h"
#include "Entities/Player/NPlayerState.h"
DEFINE_LOG_CATEGORY(LogPlayerSystem);


void FNPlayerSystem::Setup(
	const TObjectPtr<UNPlayerActionComponent>& ActionComponent,
	const TObjectPtr<ANPlayerState>& PlayerState,
	const TObjectPtr<ANPlayerController>& Controller,
	const TObjectPtr<ANPlayerCharacter>& Character,
	const TObjectPtr<UAbilitySystemComponent>& InASC)
{
		TSet<FString> InvalidComponents{};
	if(!IsValid(ActionComponent))
	{
		NPlayerActionComponent = ActionComponent;
		InvalidComponents.Add(TEXT("ActionComponent"));
	}
	if(!IsValid(PlayerState))
	{
		NPlayerState = PlayerState;
		InvalidComponents.Add(TEXT("PlayerState"));
	}
	if(!IsValid(Controller))
	{
		NPlayerController = Controller;
		InvalidComponents.Add(TEXT("Controller"));
	}
	if(IsValid(Character))
	{
		NPlayerCharacter = Character;
		InvalidComponents.Add(TEXT("PlayerCharacter"));
	}
	if(IsValid(InASC))
	{
		ASC = InASC;
		InvalidComponents.Add(TEXT("AbilitySystemComponent"));
	}
	for (auto InvalidComponent : InvalidComponents)
	{
		UE_LOG(LogPlayerSystem, Warning, TEXT("[NPlayerSystem] %s is invalid in NPS"), ToCStr(InvalidComponent));
	}
	bValid = (InvalidComponents.Num() == 0);
	
	if(IsValid(NPlayerActionComponent)) 
		NPlayerActionComponent->SetupNPS(this);
	if(IsValid(NPlayerActionComponent))
		NPlayerActionComponent->SetupNPS(this);
	if(IsValid(NPlayerState))
		NPlayerState->SetupNPS(this);
	if(IsValid(NPlayerCharacter))
		NPlayerCharacter->SetupNPS(this);
	ASC->InitAbilityActorInfo(NPlayerState, NPlayerCharacter);
	AbilityActorInfo = ASC->AbilityActorInfo;
}
