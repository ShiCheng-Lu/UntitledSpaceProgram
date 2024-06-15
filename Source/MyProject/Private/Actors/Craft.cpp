// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Craft.h"

ACraft::ACraft() {
	Root = CreateDefaultSubobject<USphereComponent>(TEXT("VisualRepresentation"));
	// Root->SetSimulatePhysics(false);
	SetRootComponent(Root);
}

TArray<UPart*> ACraft::CreatePartStructure(TSharedPtr<FJsonObject> StructureJson, UPart* StructureParent) {
	TArray<UPart*> PartList;
	for (auto& PartKVP : StructureJson->Values) {
		auto Part = NewObject<UPart>(Root);
		
		Part->Id = PartKVP.Key;
		Part->Parent = StructureParent;
		Part->Structure = PartKVP.Value->AsObject();

		PartList.Add(Part);
		// add to Craft's part list
		Parts.Add(PartKVP.Key, Part);

		Part->Children = CreatePartStructure(PartKVP.Value->AsObject(), Part);
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

	if (Root) {

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("root doesnt eixst???"));

	}

	RootPart = CreatePartStructure(Json->GetObjectField(L"structure"), nullptr)[0];
	
	for (auto& PartKVP : Json->GetObjectField(L"parts")->Values) {
		if (!Parts.Contains(PartKVP.Key)) {
			UE_LOG(LogTemp, Warning, TEXT("part key not found: %s"), *PartKVP.Key);
			continue;
		}
		
		auto Part = *Parts.Find(PartKVP.Key);
		Part->Initialize(GetRootComponent(), PartKVP.Value->AsObject());
		
		if (Part->Json->GetStringField(L"type") == "engine") {
			Engine = Part;
		}
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

void ACraft::AddPart(UPart* Part) {
	Part->Rename(*Part->GetName(), Root);
	Part->AttachToComponent(Root, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

	Parts.Add(Part->Id, Part);
	Json->GetObjectField(L"parts")->SetObjectField(Part->Id, Part->Json);
}

void ACraft::RemovePart(UPart* Part) {
	Part->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));

	Parts.Remove(Part->Id);
	Json->GetObjectField(L"parts")->RemoveField(Part->Id);
}

// transfer all parts attached to another craft
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
	NewCraft->Json->SetObjectField(L"parts", MakeShareable(new FJsonObject()));
	NewCraft->SetActorLocation(Part->GetComponentLocation());

	TransferPart(Part, this, NewCraft);
	Part->SetParent(nullptr);

	for (auto& P : NewCraft->Parts) {
		UE_LOG(LogTemp, Warning, TEXT("New Craft Part: %s @ %s - %s"), *P.Value->Id, *P.Value->GetRelativeLocation().ToString(), *P.Value->GetOwner()->GetActorGuid().ToString());
	}
	UE_LOG(LogTemp, Warning, TEXT("Old Craft Component Count: %d - %s"), GetComponents().Num(), *GetActorGuid().ToString());
	UE_LOG(LogTemp, Warning, TEXT("New Craft Component Count: %d - %s"), NewCraft->GetComponents().Num(), *NewCraft->GetActorGuid().ToString());

	return NewCraft;
}

void ACraft::UpdateJsonPartsArray() {
	TArray<TSharedPtr<FJsonValue>> parts;
	for (auto& part : Parts) {
		parts.Add(MakeShareable(new FJsonValueObject(part.Value->Json)));
	}
	Json->SetArrayField(L"parts", parts);
}

void ACraft::AttachPart(ACraft* SourceCraft, UPart* AttachToPart) {
	TransferPart(SourceCraft->RootPart, SourceCraft, this);

	// SourceCraft->Parent = AttachToPart;
	SourceCraft->RootPart->SetParent(AttachToPart);

	if (SourceCraft->Parts.IsEmpty()) {
		SourceCraft->Destroy();
	}
	else {

		UE_LOG(LogTemp, Warning, TEXT("Source craft shouldn't be empty!"));
	}
}

void ACraft::Throttle(float throttle) {
	/*
	if (Engine) {
		Engine->AddForce(FVector(0, 0, throttle));
	}
	*/
}
