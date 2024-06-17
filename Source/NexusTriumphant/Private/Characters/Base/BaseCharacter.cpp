// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/Base/BaseCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
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
#include "Characters/Base/BaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	USceneComponent* Empty = CreateDefaultSubobject<USceneComponent>(TEXT("Empty"));
	RootComponent = Empty;
	GetCapsuleComponent()->AttachToComponent(Empty,
											 FAttachmentTransformRules(
												 EAttachmentRule::KeepRelative,
												 EAttachmentRule::KeepRelative,
												 EAttachmentRule::KeepRelative, false)
	);


	
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	float HeightOffset = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, 0.f, 0.f));
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create a collider to deal with 2d collisions
	CollisionComponent = CreateDefaultSubobject<UEntityCollisionComponent>(TEXT("EntityCollisionComponent"));
	CollisionComponent->SetupCapsule(GetCapsuleComponent()->GetUnscaledCapsuleRadius());

	// Move components around so the mesh and capsule are in the correct locations
	CollisionComponent->SetRelativeLocation(FVector3d(0, 0, -HeightOffset/4));
	CollisionComponent->AttachToComponent(Empty,
							 FAttachmentTransformRules(
								 EAttachmentRule::KeepRelative,
								 EAttachmentRule::KeepRelative,
								 EAttachmentRule::KeepRelative, false)
	);
	GetCapsuleComponent()->SetRelativeLocation(FVector3d(0, 0, HeightOffset));
	GetMesh()->SetRelativeLocation(FVector3d(0, 0, -HeightOffset));
	GetMesh()->SetRelativeRotation(FQuat(0, 0, -PI / 4,PI / 4)); //-90 Z turnwell

	// Ability system items
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	InitialAbilitySet = CreateDefaultSubobject<UAbilitySet>(TEXT("InitialAbilitySet"));
	StandardAttributes = CreateDefaultSubobject<UStandardAttributeSet>(TEXT("StandardAttributeSet"));
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetupInitialAbilitiesAndEffects();
	}
}

void ABaseCharacter::SetupInitialAbilitiesAndEffects()
{
	if (IsValid(AbilitySystemComponent) == false || IsValid(StandardAttributes) == false)
	{
		return;
	}

	if (IsValid(InitialAbilitySet))
	{
		InitiallyGrantedAbilitySpecHandles.Append(
			InitialAbilitySet->UAbilitySet::GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
	}
	else
	{
		UE_LOG_ABILITY_CAUTION("Initial Ability Set is invalid", this);
	}

	if (IsValid(InitialGameplayEffect))
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

void ABaseCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Health updated"));
	if (FMath::IsNearlyZero(OnAttributeChangeData.NewValue) && OnAttributeChangeData.OldValue > 0)
	{
		
	}
}
