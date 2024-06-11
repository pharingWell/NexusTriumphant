// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Base/BaseCharacterCore.h"


// Sets default values
ABaseCharacterCore::ABaseCharacterCore()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseCharacterCore::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacterCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacterCore::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

