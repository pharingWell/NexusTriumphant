// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NPlayerCharacter.h"
#include "Entities/NexusEntity.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ANPlayerCharacter::ANPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	EntityRadius = 34.f;
	EntityHalfHeight = 88.f;
}


void ANPlayerCharacter::OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState)
{
	Super::OnPlayerStateChanged(NewPlayerState, OldPlayerState);
}

// Called when the game starts or when spawned
void ANPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
// void ANPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// }

/*
 * from tranek's GASDocumentation
 * Runs on the server
 * Client side runs at AcknowledgePossession in NPlayerController
 */

/*
 void ARBPlayerCharacter::PossessedBy(AController* NewController)
 {
 	Super::PossessedBy(NewController);
 	if (AbilitySystemComponent == nullptr)
 	{
 		if (ARBPlayerState* PS = GetPlayerState<ARBPlayerState>())
 		{
 			// Cache the ASC in the Server (TWeakObjectPtr preferrable)
 			AbilitySystemComponent = Cast<URBAbilitySystemComponent>(PS->GetAbilitySystemComponent());
 			
 			// Init the Server side part of the ASC
 			AbilitySystemComponent->InitAbilityActorInfo(PS, this);
 
 			// Some games grant attributes and abilities here
 
 			// Some games server initialize another components of the character that use the ASC here
 		}
 	}
 }	
 */

// runs on the server
void ANPlayerCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	if(NASC == nullptr)
	{
		PlayerController = Cast<ANPlayerController>(NewController);
		if (PlayerController)
		{
			NASC = PlayerController->GetNAbilitySystemComponent();
			NASC->InitAbilityActorInfo(PlayerController, this);
		}
	}

	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	SetOwner(NewController);
}

void ANPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (NASC == nullptr)
	{
		PlayerController = GetController<ANPlayerController>();
		if (PlayerController)
		{
			// Cache the ASC in the Client (TWeakObjectPtr preferrable)
			NASC = Cast<UNAbilitySystemComponent>(PlayerController->GetAbilitySystemComponent());
			
			// Init the Client side part of the ASC
			NASC->InitAbilityActorInfo(PlayerController, this);
		
			// Some games grant attributes here

			// Some games client initialize another components of the character that use the ASC here
		}
	}
	else
	{
		// Solves the data-races of controller/playerstate
		NASC->RefreshAbilityActorInfo();
	}
}


void ANPlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	// Needed in case the PC wasn't valid when we Init-ed the ASC.
	PlayerController = GetController<ANPlayerController>();
	if (PlayerController)
	{
		PlayerController->GetAbilitySystemComponent()->RefreshAbilityActorInfo();
	}
}