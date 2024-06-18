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

	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("Throttle", EKeys::I, 0.1f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping("Throttle", EKeys::K, -0.1f));
	InputComponent->BindAxis("Throttle", this, &AMyPlayerController::Throttle);
}

UPart* AMyPlayerController::PlaceHeldPart() {
	if (Craft == nullptr || SelectedPart == nullptr || Selected == nullptr) {
		return nullptr;
	}

	FVector location;
	FVector direction;
	if (!DeprojectMousePositionToWorld(location, direction)) {
		return nullptr;
	}

	FVector part_location = location + direction * PlaceDistance;
	UPart* AttachTo = nullptr;

	for (auto& node : SelectedPart->AttachmentNodes) {
		TArray<FHitResult> hit_results;
		FVector start = location;
		FVector end = (part_location + node->GetRelativeLocation() - location) * 2 + location;

		GetWorld()->LineTraceMultiByObjectType(hit_results, start, end, FCollisionObjectQueryParams::AllObjects);

		for (auto& hit_result : hit_results) {
			UAttachmentNode* component = Cast<UAttachmentNode>(hit_result.GetComponent());
			if (component == nullptr) {
				continue;
			}

			// don't check for the closer attachment node for now
			AttachTo = Cast<UPart>(component->GetOuter());
			// Actor filter don't work for some reason, maybe to do with changing component ownership with .Rename()
			if (AttachTo && component->GetOwner() != Selected) {
				part_location = AttachTo->GetComponentLocation() + component->GetRelativeLocation() - node->GetRelativeLocation();

				Selected->SetActorLocationAndRotation(part_location, FQuat::Identity);

				return AttachTo;
			}
			else {
				AttachTo = nullptr;
			}
		}
	}

	Selected->SetActorLocationAndRotation(part_location, FQuat::Identity);

	return AttachTo;
}

void AMyPlayerController::HandleClick(FKey Key) {
	if (ConstructionMode == AMyPlayerController::EditMode) {
		if (Key == EKeys::LeftMouseButton) {
			if (Selected != nullptr) {
				UPart* AttachToPart = PlaceHeldPart();
				if (AttachToPart != nullptr) {
					Craft = Cast<ACraft>(AttachToPart->GetOwner());
					Craft->AttachPart(Selected, AttachToPart);
				}
				Selected = nullptr;
				// Craft->SetAttachmentNodeVisibility(true);
				UE_LOG(LogTemp, Warning, TEXT("Something Sleeced"));

			}
			else {
				FHitResult result;
				if (GetHitResultUnderCursor(ECC_WorldStatic, true, result)) {
					SelectedPart = Cast<UPart>(result.GetComponent());

					FVector ActorLocation = SelectedPart->GetRelativeLocation();
					FVector PawnLocation = GetPawn()->GetActorLocation();
					PlaceDistance = FVector::Distance(ActorLocation, PawnLocation);

					Selected = Cast<ACraft>(SelectedPart->GetOwner());

					if (Selected->RootPart != SelectedPart) {
						Selected = Selected->DetachPart(SelectedPart);
					}

					UE_LOG(LogTemp, Warning, TEXT("Selected part %s - %s"), *SelectedPart->Id, *Selected->GetActorGuid().ToString());

					// Craft->SetAttachmentNodeVisibility(false);
				}
				UE_LOG(LogTemp, Warning, TEXT("Nothing Selected"));
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

			// DEBUG:
			if (GEngine) {
				FHitResult result;
				if (GetHitResultUnderCursor(ECC_WorldStatic, true, result)) {
					FGuid actorGuid = result.GetComponent()->GetOwner()->GetActorGuid();
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(result.GetComponent()->GetOwner()->GetUniqueID()));
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
	if (!Craft) {
		return;
	}
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


void AMyPlayerController::Throttle(float Val) {
	if (Val != 0 && Craft != nullptr) {
		Craft->Throttle(Val);
	}
}
