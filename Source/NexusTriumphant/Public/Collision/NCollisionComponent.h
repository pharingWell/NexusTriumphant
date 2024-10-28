// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "NCollisionComponent.generated.h"

UCLASS(ClassGroup=Shapes, meta=(BlueprintSpawnableComponent))

#define ECC_HALFHEIGHT 300.f

class NEXUSTRIUMPHANT_API UNCollisionComponent : public UCapsuleComponent
{
	GENERATED_BODY()

/** VARIABLES **/
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Entity")
	bool bDebugDisplayRadius;

protected:
	/** Determines the size of the entity's collision with 2d space */
	UPROPERTY(EditDefaultsOnly, Category="Entity")
	float EntityRadius;
	float HeightOffset;

/** FUNCTIONS **/
	
public:
	// Sets default values for this component's properties
	UNCollisionComponent();
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	void SetupCapsule(float Radius, float HeightOffset, bool bUpdateOverlaps=true);

	//intentionally hides function
	void SetCapsuleRadius(float Radius, bool bUpdateOverlaps=true); 
	
	void SetEntityRadius(float Radius, bool bUpdateOverlaps=true);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};
