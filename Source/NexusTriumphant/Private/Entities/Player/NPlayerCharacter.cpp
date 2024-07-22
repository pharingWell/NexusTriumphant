// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Player/NPlayerCharacter.h"

#include "Entities/NexusEntity.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ANPlayerCharacter::ANPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	PlayerState = Cast<ANPlayerState>(NewPlayerState);
}

void ANPlayerCharacter::OnSetDestinationReleased()
{
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
void ANPlayerCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);



	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	SetOwner(NewController);
}


