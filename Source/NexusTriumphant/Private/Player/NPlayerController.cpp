// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/NPlayerController.h"

#include "Entities/NexusEntity.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NexusTriumphant/NexusTriumphant.h"
#include "Player/NPlayerCharacter.h"


ANPlayerController::ANPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	bReplicates = true;
	bIsEnqueuing = false;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedMoveToDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	PlayerActionComponent = CreateDefaultSubobject<UNPlayerActionComponent>(TEXT("Player Action Component"));
	NAbilitySystemComponent = CreateDefaultSubobject<UNAbilitySystemComponent>(TEXT("NAbilitySystemComponent"));
}

void ANPlayerController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ANPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}


void ANPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	PlayerActionComponent = NewObject<UNPlayerActionComponent>(this, UNPlayerActionComponent::StaticClass());
	PlayerActionComponent->RegisterComponent();
	NAbilitySystemComponent = NewObject<UNAbilitySystemComponent>(this, UNAbilitySystemComponent::StaticClass());
	NAbilitySystemComponent->RegisterComponent();
}

void ANPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ANPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANPlayerController, NPlayerState)
}

void ANPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	NPlayerState = GetPlayerState<ANPlayerState>();
	NPlayerCharacter = Cast<ANPlayerCharacter>(P);
	if (!IsValid(PlayerActionComponent))
	{
		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Player Action Component not ready"));
		return;
	}
	if (IsValid(NPlayerState))
	{
		PlayerActionComponent->Setup(NPlayerState, this);
		NPlayerState->Server_Setup();
	}
	// {
	// 	NPlayerCharacter->SetPlayerState(NPlayerState);
	// 	if (NASC)
	// 	{
	// 		NASC->InitAbilityActorInfo(this, NPlayerCharacter);
	// 	} else
	// 	{
	// 		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] NASC not ready"));
	// 		return;
	// 	}
	// }else
	// {
	// 	UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Failed to cast player pawn (or state invalid)"));
	// }
	//...
}

void ANPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
}

void ANPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	NPlayerState = GetPlayerState<ANPlayerState>();
	if(!IsValid(NPlayerState))
	{
		UE_LOG(LogNAbilitySystem, Error, TEXT("[NPlayerController] Invalid Player State in OnRep"))
		return;
	}
	NPlayerState->Server_Setup();
	// When we're a client connected to a remote server, the player controller may replicate later than the PlayerState and AbilitySystemComponent.
	if (GetWorld()->IsNetMode(NM_Client))
	{
		if (!NAbilitySystemComponent)
			NAbilitySystemComponent = GetNAbilitySystemComponent();
		if (NAbilitySystemComponent)
			{
				// Calls InitAbilityActorInfo
				NAbilitySystemComponent->RefreshAbilityActorInfo();
				// NASC->TryActivateAbilitiesOnSpawn();
			}
		}
	}


void ANPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	EILPSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(!IsValid(EnhancedInputComponent))
	{
		UE_LOG(LogNexusTriumphant, Error, TEXT("[NPlayerController] Enhanced input component not found (failed cast)."));
		return;
	}
	if (IsValid(EILPSubsystem))
	{
		EILPSubsystem->ClearAllMappings();
		if(IsValid(NMappingContext))
		{
			// Add Input Mapping Context
			EILPSubsystem->AddMappingContext(NMappingContext, 0);
			if(NMappingContext->GetMappings().Num() < 1)
			{
				UE_LOG(LogNexusTriumphant, Warning, TEXT("[NPlayerController] Binding map is empty"));
			}
			
			for (auto& Element : NMappingContext->GetNMappings())
			{
				ENAbilityAction ActionEnum = Element.Enum;
				const UInputAction* InputAction = Element.Action;
				if(ActionEnum == ENAbilityAction::ENQUEUE)
				{
					EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ANPlayerController::EnqueueStarted);
					EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ANPlayerController::EnqueueEnded);
					EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &ANPlayerController::EnqueueEnded);
					continue;
				}
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ANPlayerController::OnInputStarted, ActionEnum);
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ANPlayerController::OnInputTriggered, ActionEnum);
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ANPlayerController::OnInputFinished, ActionEnum);
				EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &ANPlayerController::OnInputFinished, ActionEnum);
			}
			
		}else
		{
			UE_LOG(LogActionSystem, Error, TEXT("[NPlayerController] Input Definition Mapping Context is invalid."))
		}
	}else
	{
		UE_LOG(LogActionSystem, Error, TEXT("[NPlayerController] Enhanced Input Local Player is invalid."))
	}
}

void ANPlayerController::EnqueueStarted()
{
	bIsEnqueuing = true;
}

void ANPlayerController::EnqueueEnded()
{
	bIsEnqueuing = false;
}

void ANPlayerController::OnInputStarted(const ENAbilityAction InputUsed)
{
	UE_LOG(LogActionSystem, Display, TEXT("[NPlayerController] OnInputStarted AbilityAction #%d"), int(InputUsed));
	if (InputUsed == ENAbilityAction::MOVETO)
	{
		StopMovement();
	}
	
}

void ANPlayerController::OnInputTriggered(const ENAbilityAction InputUsed)
{
	UE_LOG(LogActionSystem, Display, TEXT("[NPlayerController] OnInputTriggered AbilityAction #%d"), int(InputUsed));
	
	FGameplayEventData EventData;
	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* SingleTargetHit = new FGameplayAbilityTargetData_SingleTargetHit();
	SingleTargetHit->HitResult = HitResult;
	DataHandle.Add(SingleTargetHit);
	EventData.TargetData.Append(DataHandle);
	EventData.Instigator = this;
	EventData.OptionalObject = this;
	if(!IsValid(NPlayerState)) {
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] NPlayerState invalid on trigger of #%d"), int(InputUsed));
		return;
	}
	// calling this on the action component doesn't work
	// TODO: investigate Player Action Component call path
	ExecuteAction(FNAbilityActionEntry(InputUsed, EventData));
	
	
	// PlayerActionComponent->ApplyInput(InputUsed, ENAbilityCastMode::INSTANT, bIsEnqueuing);
}

void ANPlayerController::OnInputFinished(const ENAbilityAction InputUsed)
{
	UE_LOG(LogActionSystem, Display, TEXT("OnInputFinished: Stopped using AbilityAction #%d"), int(InputUsed));

}

void ANPlayerController::PrintNetStatus()
{
	TArray<FString> options = {"ROLE_None","ROLE_SimulatedProxy", "ROLE_AutonomousProxy" , "ROLE_Authority", "ROLE_MAX"};
	UE_LOG(LogNAbilitySystem, Display, TEXT("auth %hs, local %hs, netrole %s"),
		HasAuthority() ? "true" : "false", IsLocalController() ? "true" : "false",
		*options[StaticCast<int>(GetLocalRole())])
}

void ANPlayerController::ExecuteAction(const FNAbilityActionEntry& AbilityActionEntry)
{
	PrintNetStatus();
	Server_RunAbilityAction(AbilityActionEntry);
}

void ANPlayerController::Server_RunAbilityAction_Implementation(const FNAbilityActionEntry AbilityActionEntry)
{
	PrintNetStatus();
	if(!HasAuthority())
	{
		if(IsLocalController())
		{
			UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Server functions run on client??"));
			return;
		}
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Server functions no authority"));
		return;
	}
	UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Server functions running on server"));
	if(IsValid(GetPlayerState<ANPlayerState>()) && !IsValid(NPlayerState))
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Server_RunAbilityAction: NPlayerState Corrected"));
		NPlayerState = GetPlayerState<ANPlayerState>();
	}
	if(!IsValid(NPlayerState))
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Server_RunAbilityAction: NPlayerState Invalid"));
		return;
	}
	if(!IsValid(NAbilitySystemComponent))
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Server_RunAbilityAction: ASCRef Invalid"));
		return;
	}
	if(!IsValid(PlayerActionComponent))
	{
		UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Server_RunAbilityAction: PlayerActionComponent Invalid"));
		return;
	}
	FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Ability.Used", true);
	FGameplayAbilityActorInfo* ActorInfo = NAbilitySystemComponent->AbilityActorInfo.Get();
	UAbilitySystemComponent& ASC = *NAbilitySystemComponent.Get();
	UE_LOG(LogNAbilitySystem, Warning, TEXT("Tag: %s"), *Tag.ToString())
	FGameplayAbilitySpecHandle Handle = NPlayerState->GetHandle(AbilityActionEntry.AbilityAction);
	bool Success = NAbilitySystemComponent->TriggerAbilityFromGameplayEvent(Handle, ActorInfo,
		Tag, &AbilityActionEntry.EventData, ASC);
	UE_LOG(LogActionSystem, Warning, TEXT("[NPlayerController] Triggered Ability %s with Authority: Ran %s"),
		*Handle.ToString(), Success ? TEXT("Successfully") : TEXT("Unsuccessfully"));
	auto tagContainer = NAbilitySystemComponent->GetOwnedGameplayTags();
	for(auto tag : tagContainer)
	{
		UE_LOG(LogActionSystem, Warning, TEXT("{NASC} %s"), *tag.ToString());
	}
}


bool ANPlayerController::K2_GetHitResultUnderCursor(ECollisionChannel TraceChannel, bool bTraceComplex,
                                                    FHitResult& HitResult)
{
	return GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
}


