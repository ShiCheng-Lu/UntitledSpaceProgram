// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionMode.h"
#include "ConstructionHUD.h"
#include "Player/MyPlayerController.h"
#include "Actors/MyActor.h"
#include "Player/ConstructionPawn.h"

AConstructionMode::AConstructionMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HUDClass = AConstructionHUD::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	DefaultPawnClass = AConstructionPawn::StaticClass();
}

void AConstructionMode::BeginPlay() {
	Super::BeginPlay();
}
