// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OrbitUtil.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UOrbitUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	struct OrbitState {
		FVector Position;
		FVector Velocity;

		FVector Periapsis;
		float TimeAtPeriapsis;
	};

	OrbitState GetPosition(float Periapsis, float Theta);
};
