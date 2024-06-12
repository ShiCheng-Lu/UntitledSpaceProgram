// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Craft.h"

ACraft::ACraft() {
	Root = CreateDefaultSubobject<USphereComponent>(TEXT("VisualRepresentation"));
	// StaticMesh->SetupAttachment(GetRootComponent());
	SetRootComponent(Root);

	// Initialize(JsonUtil::ReadFile(FPaths::ProjectDir() + "default_ship.json"));
	// Json->SetObjectField(L"parts", MakeShareable(new FJsonObject()));
}

TArray<APart*> ACraft::CreatePartStructure(TSharedPtr<FJsonObject> StructureJson, APart* StructureParent) {
	TArray<APart*> PartList;
	for (auto& PartKVP : StructureJson->Values) {
		APart * Part = GetWorld()->SpawnActor<APart>();
		Part->Id = PartKVP.Key;
		Part->Parent = StructureParent;
		Part->SetOwner(this);
		PartList.Add(Part);
		// add to Craft's part list
		Parts.Add(PartKVP.Key, Part);

		Part->Children = CreatePartStructure(PartKVP.Value->AsObject(), Part);
	}
	return PartList;
}

// Sets default values
void ACraft::Initialize(TSharedPtr<FJsonObject> InJson)
{
	Json = InJson;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootPart = CreatePartStructure(Json->GetObjectField("structure"), nullptr)[0];

	for (auto& PartKVP : Json->GetObjectField(L"parts")->Values) {
		auto Part = *Parts.Find(PartKVP.Key);
		Part->Initialize(PartKVP.Value->AsObject());

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

void ACraft::AddPart(APart* Part) {
	Parts.Add(Part->Id, Part);
	Json->GetObjectField(L"parts")->SetObjectField(Part->Id, Part->Json);
	Part->SetOwner(this);
}

void ACraft::RemovePart(APart* Part) {
	Parts.Remove(Part->Id);
	Json->GetObjectField(L"parts")->RemoveField(Part->Id);
}

// transfer all parts attached to another craft
void ACraft::TransferPart(APart* Part, ACraft* FromCraft, ACraft* ToCraft) {
	UE_LOG(LogTemp, Warning, TEXT("Transfer %s"), *Part->Id);
	if (!FromCraft->Parts.Contains(Part->Id)) {
		// Part has already been moved???
		UE_LOG(LogTemp, Warning, TEXT("Transfer Failed %s"), *Part->Id);
		return;
	}
	FromCraft->RemovePart(Part);
	ToCraft->AddPart(Part);

	Part->SetRelativeLocation(Part->GetActorLocation() - ToCraft->GetActorLocation());

	for (auto& Child : Part->Children) {
		TransferPart(Child, FromCraft, ToCraft);
	}
}

ACraft* ACraft::DetachPart(APart* Part) {
	ACraft* NewCraft = GetWorld()->SpawnActor<ACraft>();
	NewCraft->RootPart = Part;
	NewCraft->Json = MakeShareable(new FJsonObject());
	NewCraft->Json->SetStringField(L"name", "sub craft");
	NewCraft->Json->SetObjectField(L"parts", MakeShareable(new FJsonObject()));
	NewCraft->SetActorLocation(Part->GetActorLocation());

	TransferPart(Part, this, NewCraft);

	Part->Parent->Children.Remove(Part);
	Part->Parent = nullptr;

	for (auto& P : NewCraft->Parts) {
		UE_LOG(LogTemp, Warning, TEXT("New Craft Part: %s"), *P.Value->Id);
	}

	return NewCraft;
}

void ACraft::UpdateJsonPartsArray() {
	TArray<TSharedPtr<FJsonValue>> parts;
	for (auto& part : Parts) {
		parts.Add(MakeShareable(new FJsonValueObject(part.Value->Json)));
	}
	Json->SetArrayField(L"parts", parts);
}

void ACraft::AttachPart(ACraft* SourceCraft, APart *AttachToPart) {
	TransferPart(SourceCraft->RootPart, SourceCraft, this);

	// SourceCraft->Parent = AttachToPart;
	AttachToPart->Children.Add(SourceCraft->RootPart);
	SourceCraft->RootPart->Parent = AttachToPart;

	if (SourceCraft->Parts.IsEmpty()) {
		SourceCraft->Destroy();
	}
}

void ACraft::SetActorLocation(const FVector& NewLocation) {
	Super::SetActorLocation(NewLocation);
	for (auto& part : Parts) {
		part.Value->SetActorLocation(part.Value->GetRelativeLocation() + NewLocation);
	}
}
