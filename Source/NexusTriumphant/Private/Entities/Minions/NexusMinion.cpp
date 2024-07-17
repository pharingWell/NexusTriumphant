// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Minions/NexusMinion.h"



// Sets default values
ANexusMinion::ANexusMinion(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EntityRadius = 5.f;
	EntityHalfHeight = 55.f;
}

void ANexusMinion::PreRegisterAllComponents()
{
	Super::PreRegisterAllComponents();
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANexusMinion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANexusMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


