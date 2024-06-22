// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Entities/Base/BaseEntity.h"
#include "Minion.generated.h"

UCLASS()
class NEXUSTRIUMPHANT_API AMinion : public ABaseEntity
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinion(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
