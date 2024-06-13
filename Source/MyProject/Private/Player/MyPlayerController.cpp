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

	PlayerInput->AddActionMapping(FInputActionKeyMapping("Save", EKeys::M));
	InputComponent->BindAction("Save", IE_Pressed, this, &AMyPlayerController::Save);
	PlayerInput->AddActionMapping(FInputActionKeyMapping("Load", EKeys::N));
	InputComponent->BindAction("Load", IE_Pressed, this, &AMyPlayerController::Load);
}

APart * AMyPlayerController::PlaceHeldPart() {
	if (Craft == nullptr || SelectedPart == nullptr || Selected == nullptr) {
		return nullptr;
	}

	FVector location;
	FVector direction;
	if (DeprojectMousePositionToWorld(location, direction)) {
		FVector part_location = location + direction * PlaceDistance;
		AActor* AttachTo = nullptr;

		for (auto& node : SelectedPart->AttachmentNodes) {
			TArray<FHitResult> hit_results;
			FVector start = location;
			FVector end = (part_location + node->RelativeLocation - location) * 2 + location;
			FCollisionQueryParams query_params;

			for (auto& part : Selected->Parts) {
				query_params.AddIgnoredActor(part.Value);
			}

			GetWorld()->LineTraceMultiByObjectType(hit_results, start, end, FCollisionObjectQueryParams::AllObjects, query_params);

			for (auto& hit_result : hit_results) {
				UAttachmentNode* component = Cast<UAttachmentNode>(hit_result.GetComponent());
				if (component == nullptr) {
					continue;
				}
				// don't check for closest attachment node for now
				AttachTo = component->GetOwner();
				part_location = AttachTo->GetActorLocation() + component->RelativeLocation - node->RelativeLocation;
				break;
			}
		}

		Selected->SetActorLocation(part_location);

		if (AttachTo != nullptr) {
			return Cast<APart>(AttachTo);
		}
	}
	return nullptr;
}

void AMyPlayerController::HandleClick(FKey Key) {
	if (ConstructionMode == AMyPlayerController::EditMode) {
		if (Key == EKeys::LeftMouseButton) {
			if (SelectedPart != nullptr) {
				APart* AttachToPart = PlaceHeldPart();
				if (AttachToPart != nullptr) {
					UE_LOG(LogTemp, Warning, TEXT("Attach"));
					Cast<ACraft>(AttachToPart->GetOwner())->AttachPart(Selected, AttachToPart);
				}
				
				Selected = nullptr;
				SelectedPart = nullptr;
				// Craft->SetAttachmentNodeVisibility(true);
			}
			else {
				FHitResult result;
				if (GetHitResultUnderCursor(ECC_Visibility, true, result)) {
					SelectedPart = Cast<APart>(result.GetActor());

					FVector ActorLocation = SelectedPart->GetActorLocation();
					FVector PawnLocation = GetPawn()->GetActorLocation();
					PlaceDistance = FVector::Distance(ActorLocation, PawnLocation);

					Selected = Cast<ACraft>(SelectedPart->GetOwner());

					if (Selected->RootPart != SelectedPart) {
						Selected = Selected->DetachPart(SelectedPart);
					}

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
	else if (ConstructionMode == AMyPlayerController::RotateMode) {

	}
	else { // ConstructionMode == AMyPlayerController::TranslationMode

	}
}

void AMyPlayerController::Save() {
	JsonUtil::WriteFile(FPaths::ProjectDir() + "ship2.json", Craft->Json);
}

void AMyPlayerController::Load() {
	Craft = GetWorld()->SpawnActor<ACraft>();
	Craft->Initialize(JsonUtil::ReadFile(FPaths::ProjectDir() + "ship.json"));
}

void AMyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	switch (ConstructionMode)
	{
	case AMyPlayerController::EditMode:
		PlaceHeldPart();
		break;
	case AMyPlayerController::RotateMode:
		break;
	case AMyPlayerController::TranslateMode:
		break;
	default:
		break;
	}
}
