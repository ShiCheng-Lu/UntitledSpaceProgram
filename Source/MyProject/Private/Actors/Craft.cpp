// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Craft.h"

ACraft::ACraft() {
	Root = CreateDefaultSubobject<USphereComponent>(TEXT("VisualRepresentation"));
	// StaticMesh->SetupAttachment(GetRootComponent());
	SetRootComponent(Root);

	// Initialize(JsonUtil::ReadFile(FPaths::ProjectDir() + "default_ship.json"));
}

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
		Parts.Add(Part->Id, Part);
		UE_LOG(LogTemp, Warning, TEXT("created part"));
	}

	for (auto& Part : Parts) {
		FGuid PartParentGuid = FGuid(Part.Value->Json->GetStringField(L"attached_to"));
		if (Parts.Contains(PartParentGuid)) {
			APart* Parent = *Parts.Find(PartParentGuid);
			Parent->Children.Add(Part.Value);
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

APart* ACraft::RootPart() {
	return *Parts.Find(FGuid(Json->GetStringField(L"root")));
}

// transfer all parts attached to another craft
void TransferPart(APart* Part, ACraft* FromCraft, ACraft* ToCraft) {
	UE_LOG(LogTemp, Warning, TEXT("Transfer %s"), *Part->Id.ToString());
	if (!FromCraft->Parts.Contains(Part->Id)) {
		// Part has already been moved???
		UE_LOG(LogTemp, Warning, TEXT("Transfer Failed %s"), *Part->Id.ToString());
		return;
	}
	FromCraft->Parts.Remove(Part->Id);
	ToCraft->Parts.Add(Part->Id, Part);
	Part->SetOwner(ToCraft);

	Part->SetRelativeLocation(Part->GetActorLocation() - ToCraft->GetActorLocation());

	for (auto& Child : Part->Children) {
		TransferPart(Child, FromCraft, ToCraft);
	}
}

ACraft* ACraft::DetachPart(APart* Part) {
	ACraft* NewCraft = GetWorld()->SpawnActor<ACraft>();
	NewCraft->Json = MakeShareable(new FJsonObject());
	NewCraft->Json->SetStringField(L"name", "sub craft");
	NewCraft->Json->SetStringField(L"root", Part->Id.ToString());
	NewCraft->SetActorLocation(Part->GetActorLocation());

	TransferPart(Part, this, NewCraft);

	FGuid ParentGuid = FGuid(Part->Json->GetStringField("attached_to"));
	APart* Parent = *Parts.Find(ParentGuid);
	Parent->Children.Remove(Part);

	UpdateJsonPartsArray();
	NewCraft->UpdateJsonPartsArray();

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
	FGuid RootGuid = FGuid(SourceCraft->Json->GetStringField(L"root"));

	APart* RootPart = *SourceCraft->Parts.Find(RootGuid);
	
	TransferPart(RootPart, SourceCraft, this);

	RootPart->Json->SetStringField(L"attached_to", AttachToPart->Id.ToString());
	AttachToPart->Children.Add(RootPart);

	UpdateJsonPartsArray();
	SourceCraft->UpdateJsonPartsArray();

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
