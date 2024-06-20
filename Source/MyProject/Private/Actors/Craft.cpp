// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Craft.h"
#include "Containers/Queue.h"

ACraft::ACraft() {
}

TArray<UPart*> ACraft::CreatePartStructure(TSharedPtr<FJsonObject> StructureJson, UPart* StructureParent) {
	TArray<UPart*> PartList;
	for (auto& PartKVP : StructureJson->Values) {
		auto Part = NewObject<UPart>(this);

		auto& PartListJson = Json->GetObjectField(L"parts");
		auto& PartJson = PartListJson->GetObjectField(PartKVP.Key);
		Part->Initialize(PartKVP.Key, PartKVP.Value->AsObject(), PartJson);
		Part->SetParent(StructureParent);

		PartList.Add(Part);
		Parts.Add(PartKVP.Key, Part);

		CreatePartStructure(PartKVP.Value->AsObject(), Part);
		UE_LOG(LogTemp, Warning, TEXT("created: %s"), *PartKVP.Key);
	}
	return PartList;
}

// Sets default values
void ACraft::Initialize(TSharedPtr<FJsonObject> InJson)
{
	Json = InJson;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TArray<TPair<UPart*, TSharedPtr<FJsonObject>>> PartStructures = { { nullptr, Json->GetObjectField(TEXT("structure")) } };
	for (int i = 0; i < PartStructures.Num(); ++i) {
		for (auto& PartKVP : PartStructures[i].Value->Values) {
			auto Part = NewObject<UPart>(this);

			auto& PartListJson = Json->GetObjectField(L"parts");
			auto& PartJson = PartListJson->GetObjectField(PartKVP.Key);
			Part->Initialize(PartKVP.Key, PartKVP.Value->AsObject(), PartJson);
			Part->SetParent(PartStructures[i].Key);

			Parts.Add(PartKVP.Key, Part);

			PartStructures.Add({ Part, PartKVP.Value->AsObject() });
			UE_LOG(LogTemp, Warning, TEXT("created: %s"), *PartKVP.Key);
		}
	}

	RootPart = *Parts.Find(PartStructures[0].Value->Values.begin().Key());
	
	if (true) { // building
		SetRootComponent(RootPart);
		for (auto& P : Parts) {
			P.Value->AttachToComponent(P.Value == RootPart ? nullptr : RootPart, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
		}
	}

	Engine = *Parts.Find("engine-1");
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

void ACraft::AddPart(UPart* Part) {
	Parts.Add(Part->Id, Part);
	Json->GetObjectField(L"parts")->SetObjectField(Part->Id, Part->Json);
	// change ownership
	Part->Rename(*Part->GetName(), this);
}

void ACraft::RemovePart(UPart* Part) {
	Parts.Remove(Part->Id);
	Json->GetObjectField(L"parts")->RemoveField(Part->Id);
}

// transfer ownership of part and any children to another craft
void ACraft::TransferPart(UPart* Part, ACraft* FromCraft, ACraft* ToCraft) {
	UE_LOG(LogTemp, Warning, TEXT("Transfer %s"), *Part->Id);
	if (!FromCraft->Parts.Contains(Part->Id)) {
		// Part has already been moved???
		UE_LOG(LogTemp, Warning, TEXT("Transfer Failed %s"), *Part->Id);
		return;
	}
	FromCraft->RemovePart(Part);
	ToCraft->AddPart(Part);

	for (auto& Child : Part->Children) {
		TransferPart(Child, FromCraft, ToCraft);
	}
}

ACraft* ACraft::DetachPart(UPart* Part) {
	ACraft* NewCraft = GetWorld()->SpawnActor<ACraft>();
	NewCraft->RootPart = Part;
	NewCraft->Json = MakeShareable(new FJsonObject());
	NewCraft->Json->SetStringField(L"name", "sub craft");
	TSharedPtr<FJsonObject> structure = MakeShareable(new FJsonObject());
	structure->SetObjectField(Part->Id, Part->Structure);
	NewCraft->Json->SetObjectField(L"structure", structure);
	NewCraft->Json->SetObjectField(L"parts", MakeShareable(new FJsonObject()));

	NewCraft->SetActorLocation(Part->GetComponentLocation());

	TransferPart(Part, this, NewCraft);
	
	Part->SetParent(nullptr);
	NewCraft->SetRootComponent(Part);

	if (true) { // building
		for (auto& PartKVP : NewCraft->Parts) {
			auto& Part = PartKVP.Value;
			if (Part == NewCraft->RootPart) {
				Part->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
			}
			else {
				Part->AttachToComponent(NewCraft->RootPart, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
			}
		}
	}

	return NewCraft;
}

void ACraft::AttachPart(ACraft* SourceCraft, UPart* AttachToPart) {
	TArray<UPart*> SourceParts;
	for (auto& PartKVP : SourceCraft->Parts) {
		SourceParts.Add(PartKVP.Value);
	}

	TransferPart(SourceCraft->RootPart, SourceCraft, this);

	SourceCraft->RootPart->SetParent(AttachToPart);

	if (SourceCraft->Parts.IsEmpty()) {
		SourceCraft->Destroy();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Source craft should be empty but is not!"));
	}
	
	for (auto& Part : SourceParts) {
		Part->AttachToComponent(RootPart, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	}
}

void ACraft::Throttle(float throttle) {
	if (Engine) {
		Engine->AddForce(FVector(0, 0, 10000));
	}
}
