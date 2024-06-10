// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Actors/Part.h"
#include "Actors/Craft.h"
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

	void HandleClick(FKey Key);

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;

	float PlaceDistance;
protected:
	APart* SelectedPart;
	ACraft* Craft;
	ACraft* Selected;

	enum Mode {
		EditMode,
		RotateMode,
		TranslateMode,
	};

	Mode ConstructionMode = Mode::EditMode;
};
