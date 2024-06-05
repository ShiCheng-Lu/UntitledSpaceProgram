// Fill out your copyright notice in the Description page of Project Settings.

#include "ConstructionHUD.h"
#include "Actors/Part.h"
#include "Player/MyPlayerController.h"

AMyPlayerController::AMyPlayerController() {
	SetShowMouseCursor(true);
	bEnableClickEvents = true;



	if (InputEnabled()) {
		UE_LOG(LogTemp, Warning, TEXT("Input enabled"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Input not enabled"));

	}

	// add right mouse button to click event as well
	ClickEventKeys.Add(EKeys::RightMouseButton);
}

bool AMyPlayerController::InputKey(const FInputKeyParams& Params) {
	bool handled = Super::InputKey(Params);

	UE_LOG(LogTemp, Warning, TEXT("Input Key Hit"));
	if (Params.Key == EKeys::LeftMouseButton && Params.Event == IE_Pressed) {
		UE_LOG(LogTemp, Warning, TEXT("Left clicked"));
		if (SelectedPart != NULL) {
			// place the part
			SelectedPart = NULL;
		}
		else {
			FHitResult result;
			if (GetHitResultUnderCursor(ECC_Visibility, true, result)) {
				SelectedPart = (APart*)result.GetActor();
				FVector ActorLocation = SelectedPart->GetActorLocation();
				FVector PawnLocation = GetPawn()->GetActorLocation();
				PlaceDistance = FVector::Distance(ActorLocation, PawnLocation);
			}
		}

	}
	else if (Params.Key == EKeys::RightMouseButton && Params.Event == IE_Released) {
		UE_LOG(LogTemp, Warning, TEXT("Right clicked"));
		// ignore if holding a part
		if (SelectedPart == NULL) {
			FHitResult result;
			if (GetHitResultUnderCursor(ECC_Visibility, true, result)) {
				TSharedPtr<FJsonObject> Part = ((APart*)result.GetActor())->Json;
				((AConstructionHUD*)MyHUD)->MyWidget->ShowPart(Part);
			}
		}
	}

	return handled;
}

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();
}

void AMyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	if (SelectedPart != NULL) {
		FVector location;
		FVector direction;
		if (DeprojectMousePositionToWorld(location, direction)) {
			SelectedPart->SetActorLocation(location + direction * PlaceDistance);
		}
	}
}
