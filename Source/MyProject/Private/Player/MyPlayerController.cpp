// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/MyPlayerController.h"

#include "ConstructionHUD.h"
#include "Actors/Part.h"
#include "Kismet/GameplayStatics.h"

AMyPlayerController::AMyPlayerController() {

	SetShowMouseCursor(true);
	bEnableClickEvents = true;

	// add right mouse button to click event as well
	ClickEventKeys.Add(EKeys::RightMouseButton);
}

void AMyPlayerController::HandleClick(FKey Key) {
	if (Key == EKeys::LeftMouseButton) {
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

				UE_LOG(LogTemp, Warning, TEXT("Hit part"));
			}
		}
	}
	else if (Key == EKeys::RightMouseButton) {
		UE_LOG(LogTemp, Warning, TEXT("Right clicked"));
		// ignore if holding a part
		if (SelectedPart == NULL) {
			/*
			FHitResult result;
			if (GetHitResultUnderCursor(ECC_Visibility, true, result)) {
				TSharedPtr<FJsonObject> Part = ((APart*)result.GetActor())->Json;
				((AConstructionHUD*)MyHUD)->MyWidget->ShowPart(Part);
			}
			*/
		}
	}
}

void AMyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	if (SelectedPart != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Tick"));
		FVector location;
		FVector direction;
		if (DeprojectMousePositionToWorld(location, direction)) {
			SelectedPart->SetActorLocation(location + direction * PlaceDistance);
		}
	}
}
