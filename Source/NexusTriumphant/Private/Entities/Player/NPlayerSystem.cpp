#include "Entities/Player/NPlayerSystem.h"
#include "Entities//Player/NPlayerController.h"
#include "Entities/Player/NPlayerActionComponent.h"
#include "Entities/Player/NPlayerCharacter.h"
#include "Entities/Player/NPlayerState.h"
DEFINE_LOG_CATEGORY(LogPlayerSystem);

UNPlayerSystem::UNPlayerSystem(const FObjectInitializer&)
{
	
}

void UNPlayerSystem::Setup(
	TObjectPtr<UNPlayerActionComponent> ActionComponent,
	TObjectPtr<APlayerState> PlayerState,
	TObjectPtr<ANPlayerController> Controller,
	TObjectPtr<APawn> Pawn,
	TObjectPtr<UAbilitySystemComponent> InASC)
{
	TSet<FString> InvalidComponents{};
	if(IsValid(ActionComponent)){
		NPlayerActionComponent = ActionComponent;}
	else
		InvalidComponents.Add(TEXT("ActionComponent"));
	
	if(IsValid(PlayerState)){
		NPlayerState = Cast<ANPlayerState>(PlayerState);}
	else
		InvalidComponents.Add(TEXT("PlayerState"));
	
	if(IsValid(Controller)){
		NPlayerController = Controller;}
	else
		InvalidComponents.Add(TEXT("Controller"));
	
	if(IsValid(Pawn)){
		NPlayerCharacter = Cast<ANPlayerCharacter>(Pawn);}
	else
		InvalidComponents.Add(TEXT("PlayerCharacter"));
	
	if(IsValid(InASC)){
		ASC = InASC;}
	else
		InvalidComponents.Add(TEXT("AbilitySystemComponent"));
	
	if(IsValid(NPlayerActionComponent)) 
		NPlayerActionComponent->SetupNPS(this);
	if(IsValid(NPlayerActionComponent))
		NPlayerActionComponent->SetupNPS(this);
	if(IsValid(NPlayerState))
		NPlayerState->SetupNPS(this);
	if(IsValid(NPlayerCharacter))
		NPlayerCharacter->SetupNPS(this);
	for (auto InvalidComponent : InvalidComponents)
	{
		UE_LOG(LogPlayerSystem, Warning, TEXT("[NPlayerSystem] %s is invalid in NPS"), ToCStr(InvalidComponent));
	}
	bValid = (InvalidComponents.Num() == 0);
}
