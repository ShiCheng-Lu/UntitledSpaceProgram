// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Craft.h"

// Sets default values
void ACraft::Initialize(TSharedPtr<FJsonObject> InJson)
{
	Json = InJson;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (auto& PartJson : Json->GetArrayField(L"parts")) {
		APart* Part = GetWorld()->SpawnActor<APart>();
		Part->SetOwner(this);
		Part->Initialize(PartJson->AsObject());
		Parts.Add(FGuid::NewGuid(), Part);
		UE_LOG(LogTemp, Warning, TEXT("created part"));
	}
}

// Called when the game starts or when spawned
void ACraft::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frameda
void ACraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACraft::SetAttachmentNodeVisibility(bool visibility) {
	for (auto& part : Parts) {
		part.Value->SetAttachmentNodeVisibility(visibility);
	}
}
