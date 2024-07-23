// Copyright Epic Games, Inc. All Rights Reserved.


#include "Entities/NexusEntity.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY(LogBaseEntity);

 
ANexusEntity::ANexusEntity(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	CollisionComponent = CreateDefaultSubobject<UNCollisionComponent>(TEXT("EntityCollisionComponent"));
	EntityRadius = 50.f;
	EntityHalfHeight = 100.f;
	GetMesh()->SetRelativeLocation(FVector(0,0,-EntityHalfHeight));
	GetMesh()->SetRelativeRotation(FQuat(0, 0, -PI / 4,PI / 4)); //-90 Z turn
}

void ANexusEntity::PreRegisterAllComponents()
{
	Super::PreInitializeComponents();
	GetCapsuleComponent()->InitCapsuleSize(EntityRadius, EntityHalfHeight);
	GetCapsuleComponent()->UPrimitiveComponent::SetCollisionProfileName("Entity");
	//GetCapsuleComponent()->SetRelativeLocation(FVector(0,0,EntityHalfHeight));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// Create Entity Collision Component
	
	check(IsValid(CollisionComponent))
	CollisionComponent->SetupCapsule(EntityRadius, EntityHalfHeight);
	CollisionComponent->SetupAttachment(GetCapsuleComponent());



	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	//GetCharacterMovement()->
	//GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, 0.f, 1.f));
	//GetCharacterMovement()->SetPlaneConstraintOrigin(FVector(0.f, 0.f, EntityHalfHeight));
	//GetCharacterMovement()->bConstrainToPlane = true;
	//GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Create a collider to deal with 2d collisions
	// Move components around so the mesh and capsule are in the correct locations

}

void ANexusEntity::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->SetPlaneConstraintOrigin(GetActorLocation());
	OnDebugVisualizationDisable.AddUObject(this, &ANexusEntity::DebugVisualizationsDisable);
	DebugVisualizationsDisable();
}

void ANexusEntity::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_STRING_CHECKED(ANexusEntity, EntityHalfHeight))
	{
		GetCapsuleComponent()->InitCapsuleSize(EntityRadius, EntityHalfHeight);
		CollisionComponent->SetupCapsule(EntityRadius, EntityHalfHeight);
		GetCharacterMovement()->SetPlaneConstraintOrigin(FVector(0.f, 0.f, EntityHalfHeight));
		GetMesh()->SetRelativeLocation(FVector3d(0, 0, -EntityHalfHeight));
	}
}

void ANexusEntity::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ANexusEntity::DebugVisualizationsToggle(const FString modeString)
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

//for spawning entities on the ground
FVector ANexusEntity::GetRootLocationOffset()
{
	return FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

FVector ANexusEntity::GetRootLocationOffset(const FVector& InVector) const
{
	return InVector + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void ANexusEntity::DebugVisualizationsDisable() const
{
	CollisionComponent->bDebugDisplayRadius = false;
}

void ANexusEntity::SetCapsuleRadius(float InRadius)
{
	EntityRadius = InRadius;
	GetCapsuleComponent()->SetCapsuleRadius(EntityRadius);
}


