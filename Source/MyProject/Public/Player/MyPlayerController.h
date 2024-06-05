// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Actors/Part.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	// handle keypress, return true if there is an action that can handle the key (if key was handled)
	virtual bool InputKey(const FInputKeyParams& Params) override;

	virtual void PlayerTick(float DeltaTime) override;

	virtual void BeginPlay() override;

	float PlaceDistance;
protected:
	APart* SelectedPart;
};
