// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "Collision/NCollisionComponent.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "NexusEntity.generated.h"



// bitmaskable
UENUM(meta=(Bitflags))
enum EBaseEntityVisualizeMode
{
	Disabled    = 0b0    UMETA(DisplayName="Disabled"),
	Collision2D = 0b1    UMETA(DisplayName="Collision2D"),
	Other       = 0b10   UMETA(DisplayName="Other"),
	
};

ENUM_CLASS_FLAGS(EBaseEntityVisualizeMode);
ENUM_RANGE_BY_FIRST_AND_LAST(EBaseEntityVisualizeMode, EBaseEntityVisualizeMode::Disabled, EBaseEntityVisualizeMode::Other);

static EBaseEntityVisualizeMode DebugVisualizeMode {EBaseEntityVisualizeMode::Collision2D};

DECLARE_MULTICAST_DELEGATE(FDebugVisualizationDelegate)
static FDebugVisualizationDelegate OnDebugVisualizationDisable;
DECLARE_LOG_CATEGORY_EXTERN(LogBaseEntity, Verbose, All);


UCLASS(Blueprintable)
class ANexusEntity : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UNCollisionComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Entity")
	float EntityRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Entity")
	float EntityHalfHeight;

public:
	ANexusEntity(const FObjectInitializer& ObjectInitializer);
	virtual void PreRegisterAllComponents() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	UFUNCTION(Exec, Category="Commands")
	void DebugVisualizationsToggle(const FString modeString);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Transform")
	FVector GetRootLocationOffset();
	FVector GetRootLocationOffset(const FVector& InVector) const;

	void DebugVisualizationsDisable() const;
	void SetCapsuleRadius(float InRadius);
	



	

	


};
