// Copyright Epic Games, Inc. All Rights Reserved.


#include "Characters/Base/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/AbilitySet.h"
#include "Characters/Attributes/StandardAttributeSet.h"
#include "Abilities/AbilityHelpers.h"


 
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UCapsuleComponent* CapsuleComponentRemade = GetCapsuleComponent();
	CapsuleComponentRemade->InitCapsuleSize(42.f, 96.0f);
	
	// Set size for player capsule
	float HeightOffset = CapsuleComponentRemade->GetScaledCapsuleHalfHeight();
	CapsuleComponentRemade->SetRelativeLocation(FVector3d(0, 0, HeightOffset));

	// Create Entity Collision Component
	CollisionComponent = CreateDefaultSubobject<UEntityCollisionComponent>(TEXT("EntityCollisionComponent"));
	check(IsValid(CollisionComponent))
	CollisionComponent->SetupCapsule(42.f, HeightOffset);
	CollisionComponent->SetupAttachment(CapsuleComponentRemade);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, 0.f, 1.f));
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(CapsuleComponentRemade);
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
	// Move components around so the mesh and capsule are in the correct locations
	GetMesh()->SetRelativeLocation(FVector3d(0, 0, -HeightOffset));
	GetMesh()->SetRelativeRotation(FQuat(0, 0, -PI / 4,PI / 4)); //-90 Z turn
	
	// Ability system items
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	InitialAbilitySet = CreateDefaultSubobject<UAbilitySet>(TEXT("InitialAbilitySet"));
	StandardAttributes = CreateDefaultSubobject<UStandardAttributeSet>(TEXT("StandardAttributeSet"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetupInitialAbilitiesAndEffects();
	}
	
	GetCharacterMovement()->SetPlaneConstraintOrigin(GetActorLocation());
	if(CollisionComponent->bDebugDisplayRadius)
	{
		DebugVisualizeMode |= EBaseCharacterVisualizeMode::Collision2D;
	}
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UE_LOG(LogBaseCharacter, Display, L"%d", DebugVisualizeMode);
	if(DebugVisualizeMode != 0x0)
	{
		typedef int32 modeType;
		modeType x = StaticCast<modeType>(DebugVisualizeMode);
		for (modeType bit = 1; x >= bit; bit *= 2) if (x & bit) switch (StaticCast<EBaseCharacterVisualizeMode>(bit))
		{
		case EBaseCharacterVisualizeMode::Collision2D:
			CollisionComponent->bDebugDisplayRadius = true;
			break;
		default:
			break;
		}
	}else
	{
		CollisionComponent->bDebugDisplayRadius = false;
		UE_LOG(LogBaseCharacter, Display, L"False");
	}
}

 void ABaseCharacter::DebugToggleVisualizations(const FString modeString)
{

	for (const auto VisualizeMode : TEnumRange<EBaseCharacterVisualizeMode>())
	{
		if(UEnum::GetDisplayValueAsText<EBaseCharacterVisualizeMode>(VisualizeMode).ToString()
			== modeString)
		{
			auto temp = DebugVisualizeMode;
			DebugVisualizeMode ^= VisualizeMode;
			break;
		}
		
	}
	
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, StandardAttributes);
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
