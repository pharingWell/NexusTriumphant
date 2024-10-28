// Fill out your copyright notice in the Description page of Project Settings.


#include "Collision/NCollisionComponent.h"
#include "Entities/NexusEntity.h"


// Sets default values for this component's properties
UNCollisionComponent::UNCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	CanCharacterStepUpOn = ECB_No;
	this->UPrimitiveComponent::SetCollisionProfileName("GameplayAbilityEntity");
	this->bDynamicObstacle = false;
	this->bCanEverAffectNavigation = false;
	
	// ...
}

#if WITH_EDITOR
void UNCollisionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(PropertyChangedEvent.GetPropertyName() != "")
	{
		//UE_LOG(LogBaseEntity, Warning, TEXT("[%s] %s"), ToCStr(this->GetFullName()),  ToCStr(PropertyChangedEvent.GetPropertyName().ToString()));
		if(PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_STRING_CHECKED(UNCollisionComponent, EntityRadius)  
		|| PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_STRING_CHECKED(UNCollisionComponent, CapsuleRadius)  
		){
			Super::SetCapsuleRadius(EntityRadius);
			if(GetOwner()->IsA(ANexusEntity::StaticClass()))
			{
				if(ANexusEntity* OwningEntity = Cast<ANexusEntity>(GetOwner()))
				{
					OwningEntity->SetCapsuleRadius(EntityRadius);
				}
			}
		}
	}
}
#endif

void UNCollisionComponent::SetupCapsule(float Radius, float InHeightOffset, bool bUpdateOverlaps)
{
	EntityRadius = Radius;
	this->HeightOffset = InHeightOffset;
	InitCapsuleSize(Radius, ECC_HALFHEIGHT);
}


void UNCollisionComponent::SetCapsuleRadius(float Radius, bool bUpdateOverlaps)
{
	EntityRadius = Radius;
	Super::SetCapsuleRadius(Radius);
}

void UNCollisionComponent::SetEntityRadius(float Radius, bool bUpdateOverlaps)
{
	EntityRadius = Radius;
	Super::SetCapsuleRadius(Radius, bUpdateOverlaps);
}

// Called when the game starts
void UNCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	//SetEntityRadius(EntityRadius);
}


// Called every frame
void UNCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	if(this->bDebugDisplayRadius)
	{
		DrawCircle(GetWorld(), GetComponentLocation() - FVector(0,0,HeightOffset),
			FVector(1.f, 0.f, 0.f),
			FVector(0.f, 1.f, 0.f),
			FColor::Purple, EntityRadius, 40,
			false, DeltaTime);
	}
	// ...
}

