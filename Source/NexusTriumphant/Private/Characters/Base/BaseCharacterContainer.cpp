// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Base/BaseCharacterContainer.h"


// Sets default values
ACharacterContainer::ACharacterContainer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACharacterContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterContainer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

