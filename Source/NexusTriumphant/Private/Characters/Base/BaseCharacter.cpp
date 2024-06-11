// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/Base/BaseCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "AbilitySystemComponent.h"
#include "Abilities/AbilitySet.h"
#include "Characters/Attributes/StandardAttributeSet.h"
#include "Abilities/AbilityHelpers.h"

ANexusTriumphantCharacter::ANexusTriumphantCharacter(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(0.f, -60.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	InitialAbilitySet = CreateDefaultSubobject<UAbilitySet>(TEXT("InitialAbilitySet"));
	StandardAttributes = CreateDefaultSubobject<UStandardAttributeSet>(TEXT("StandardAttributeSet"));
}

void ANexusTriumphantCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ANexusTriumphantCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		SetupInitialAbilitiesAndEffects();
	}
}

void ANexusTriumphantCharacter::SetupInitialAbilitiesAndEffects()
{
	if(IsValid(AbilitySystemComponent) == false || IsValid(StandardAttributes) == false)
	{
		return;
	}

	if(IsValid(InitialAbilitySet))
	{
		InitiallyGrantedAbilitySpecHandles.Append(InitialAbilitySet->UAbilitySet::GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
	}
	else
	{
		UE_LOG_ABILITY_CAUTION("Initial Ability Set is invalid", this);
	}
	
	if(IsValid(InitialGameplayEffect))
	{
	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(),
		0.f,
		AbilitySystemComponent->MakeEffectContext()
	);
	}
	else
	{
		UE_LOG_ABILITY_CAUTION("Initial Ability Effect is invalid", this);
	}


	//AbilitySystemComponent->ApplyGameplayEffectToSelf(CreateDefaultSubobject<UGameplayEffect>())
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStandardAttributeSet::GetHealthAttribute())
	.AddUObject(this, &ThisClass::OnHealthAttributeChanged);
}

void ANexusTriumphantCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Health updated"));
	if(FMath::IsNearlyZero(OnAttributeChangeData.NewValue) && OnAttributeChangeData.OldValue > 0)
	{
		
	}
}