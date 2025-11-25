// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEXUSTRIUMPHANT_API UNAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UNAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);
};
