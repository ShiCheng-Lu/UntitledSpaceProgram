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

void AMyPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveForwardBackward", EKeys::W, 1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveForwardBackward", EKeys::S, -1.f));

	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveLeftRight", EKeys::A, -1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveLeftRight", EKeys::D, 1.f));

	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveUpDown", EKeys::SpaceBar, 1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("MoveUpDown", EKeys::LeftShift, -1.f));

	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("LookX", EKeys::MouseX, 1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("LookY", EKeys::MouseY, -1.f));

	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("ZoomIn", EKeys::MouseWheelAxis, 100.f));

	PlayerInput->AddActionMapping(FInputActionKeyMapping("LeftClick", EKeys::LeftMouseButton));
	PlayerInput->AddActionMapping(FInputActionKeyMapping("RightClick", EKeys::RightMouseButton));
	PlayerInput->AddActionMapping(FInputActionKeyMapping("MiddleClick", EKeys::MiddleMouseButton));


	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMyPlayerController::HandleClick);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AMyPlayerController::HandleClick);
	InputComponent->BindAction("MiddleClick", IE_Pressed, this, &AMyPlayerController::HandleClick);
}

void AMyPlayerController::HandleClick(FKey Key) {
	if (Key == EKeys::LeftMouseButton) {
		if (SelectedPart != NULL) {
			// place the part
			SelectedPart = NULL;
			// Craft->SetAttachmentNodeVisibility(true);
		}
		else {
			FHitResult result;
			if (GetHitResultUnderCursor(ECC_Visibility, true, result)) {
				SelectedPart = (APart*)result.GetActor();
				FVector ActorLocation = SelectedPart->GetActorLocation();
				FVector PawnLocation = GetPawn()->GetActorLocation();
				PlaceDistance = FVector::Distance(ActorLocation, PawnLocation);

				UE_LOG(LogTemp, Warning, TEXT("Hit part"));

				// Craft->SetAttachmentNodeVisibility(false);
			}
		}
	}
	else if (Key == EKeys::RightMouseButton) {
		UE_LOG(LogTemp, Warning, TEXT("Right clicked"));
		// ignore if holding a part
		if (SelectedPart == NULL) {
			FHitResult result;
			if (GetHitResultUnderCursor(ECC_Visibility, true, result)) {
				// TSharedPtr<FJsonObject> Part = ((APart*)result.GetActor())->Json;
				// ((AConstructionHUD*)MyHUD)->MyWidget->ShowPart(Part);
			}
		}
		
	}
}

void AMyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	if (SelectedPart != NULL) {
		FVector location;
		FVector direction;
		if (DeprojectMousePositionToWorld(location, direction)) {
			FVector part_location = location + direction * PlaceDistance;

			for (auto& node : SelectedPart->AttachmentNodes) {
				TArray<FHitResult> hit_results;
				FVector start = location;
				FVector end = (part_location + node->RelativeLocation - location) * 2 + location;
				FCollisionQueryParams query_params;
				query_params.AddIgnoredActor(SelectedPart);
				GetWorld()->LineTraceMultiByObjectType(hit_results, start, end, FCollisionObjectQueryParams::AllObjects, query_params);

				for (auto& hit_result : hit_results) {
					UAttachmentNode*component = Cast<UAttachmentNode>(hit_result.GetComponent());
					if (component == nullptr) {
						continue;
					}
					// don't check for closest attachment node for now
					part_location = component->GetOwner()->GetActorLocation() + component->RelativeLocation - node->RelativeLocation;
				}
			}

			SelectedPart->SetActorLocation(part_location);
		}
	}
}
