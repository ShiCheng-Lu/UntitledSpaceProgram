// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ConstructionMode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AConstructionMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AConstructionMode(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
};
