// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "EntityCollisionComponent.generated.h"

UCLASS(ClassGroup=Shapes, meta=(BlueprintSpawnableComponent))

#define ECC_HALFHEIGHT 85.f

class NEXUSTRIUMPHANT_API UEntityCollisionComponent : public UCapsuleComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UEntityCollisionComponent();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void SetupCapsule(float Radius, bool bUpdateOverlaps=true);
	void SetCapsuleRadius(float Radius, bool bUpdateOverlaps=true); //intentionally hides function
	void SetEntityRadius(float Radius, bool bUpdateOverlaps=true);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Determines the size of the entity's collision with 2d space */
	UPROPERTY(EditDefaultsOnly, Category="Entity")
	float EntityRadius;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
