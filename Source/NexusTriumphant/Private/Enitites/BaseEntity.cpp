// Copyright Epic Games, Inc. All Rights Reserved.


#include "Entities/BaseEntity.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"



 
ABaseEntity::ABaseEntity(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GetCapsuleComponent()->InitCapsuleSize(EntityRadius, HeightOffset);


	// Create Entity Collision Component
	CollisionComponent = CreateDefaultSubobject<UEntityCollisionComponent>(TEXT("EntityCollisionComponent"));
	check(IsValid(CollisionComponent))
	CollisionComponent->SetupCapsule(EntityRadius, HeightOffset);
	CollisionComponent->SetupAttachment(GetCapsuleComponent());

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	//GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, 0.f, 1.f));
	//GetCharacterMovement()->SetPlaneConstraintOrigin(FVector(0.f, 0.f, HeightOffset));
	//GetCharacterMovement()->bConstrainToPlane = true;
	//GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Create a collider to deal with 2d collisions
	// Move components around so the mesh and capsule are in the correct locations
	GetMesh()->SetRelativeLocation(FVector3d(0, 0, -HeightOffset));
	GetMesh()->SetRelativeRotation(FQuat(0, 0, -PI / 4,PI / 4)); //-90 Z turn

}

void ABaseEntity::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->SetPlaneConstraintOrigin(GetActorLocation());
	OnDebugVisualizationDisable.AddUObject(this, &ABaseEntity::DebugVisualizationsDisable);
	DebugVisualizationsDisable();
}

void ABaseEntity::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_STRING_CHECKED(ABaseEntity, HeightOffset))
	{
		GetCapsuleComponent()->InitCapsuleSize(EntityRadius, HeightOffset);
		CollisionComponent->SetupCapsule(EntityRadius, HeightOffset);
		GetCharacterMovement()->SetPlaneConstraintOrigin(FVector(0.f, 0.f, HeightOffset));
		GetMesh()->SetRelativeLocation(FVector3d(0, 0, -HeightOffset));
	}
}

void ABaseEntity::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseEntity::DebugVisualizationsToggle(const FString modeString)
{

	for (const auto VisualizeMode : TEnumRange<EBaseEntityVisualizeMode>())
	{
		if(UEnum::GetDisplayValueAsText<EBaseEntityVisualizeMode>(VisualizeMode).ToString()
			== modeString)
		{
			auto temp = DebugVisualizeMode;
			DebugVisualizeMode ^= VisualizeMode;
			break;
		}
		
	}
	if(DebugVisualizeMode == 0x0)
	{
		OnDebugVisualizationDisable.Broadcast();
	}else
	{
		typedef int32 modeType;
		modeType x = StaticCast<modeType>(DebugVisualizeMode);
		for (modeType bit = 1; x >= bit; bit *= 2) if (x & bit) switch (StaticCast<EBaseEntityVisualizeMode>(bit))
		{
		case EBaseEntityVisualizeMode::Collision2D:
			if(!CollisionComponent->bDebugDisplayRadius)
				CollisionComponent->bDebugDisplayRadius = true;
			break;
		default:
			break;
		}
	}
}

void ABaseEntity::DebugVisualizationsDisable() const
{
	CollisionComponent->bDebugDisplayRadius = false;
}

void ABaseEntity::SetCapsuleRadius(float InRadius)
{
	EntityRadius = InRadius;
	GetCapsuleComponent()->SetCapsuleRadius(EntityRadius);
}


