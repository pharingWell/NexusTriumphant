// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityCollisionComponent.h"

#include "Characters/Base/BasePlayerController.h"


// Sets default values for this component's properties
UEntityCollisionComponent::UEntityCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

#if WITH_EDITOR
void UEntityCollisionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if(PropertyChangedEvent.GetPropertyName() != "")
	{
		//UE_LOG(LogBaseCharacter, Warning, TEXT("[%s] %s"), ToCStr(this->GetFullName()),  ToCStr(PropertyChangedEvent.GetPropertyName().ToString()));
		if(PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_STRING_CHECKED(UEntityCollisionComponent, EntityRadius)  
		|| PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_STRING_CHECKED(UEntityCollisionComponent, CapsuleRadius)  
		){
			Super::SetCapsuleRadius(EntityRadius);
		}
	}
}

void UEntityCollisionComponent::SetupCapsule(float Radius, float InHeightOffset, bool bUpdateOverlaps)
{
	EntityRadius = Radius;
	this->HeightOffset = InHeightOffset;
	InitCapsuleSize(Radius, ECC_HALFHEIGHT);
}
#endif

void UEntityCollisionComponent::SetCapsuleRadius(float Radius, bool bUpdateOverlaps)
{
	EntityRadius = Radius;
	Super::SetCapsuleRadius(Radius);
}

void UEntityCollisionComponent::SetEntityRadius(float Radius, bool bUpdateOverlaps)
{
	EntityRadius = Radius;
	Super::SetCapsuleRadius(Radius, bUpdateOverlaps);
}

// Called when the game starts
void UEntityCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	//SetEntityRadius(EntityRadius);
}


// Called every frame
void UEntityCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bDebugDisplayRadius)
	{
		DrawCircle(GetWorld(), GetComponentLocation() - FVector(0,0,HeightOffset),
			FVector(1.f, 0.f, 0.f),
			FVector(0.f, 1.f, 0.f),
			FColor::Purple, EntityRadius, 40,
			false, DeltaTime);
	}
	// ...
}

