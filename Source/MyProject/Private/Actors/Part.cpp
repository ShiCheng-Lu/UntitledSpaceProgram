// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Part.h"
#include "Engine/StreamableManager.h"
#include "AssetLibrary.h"
#include "Engine/StaticMesh.h"
#include "Actors/AttachmentNode.h"

APart::APart() {
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	// StaticMesh->SetupAttachment(GetRootComponent());
	SetRootComponent(StaticMesh);
}

// Sets default values
void APart::Initialize(TSharedPtr<FJsonObject> InJson)
{
	Json = InJson;
	Id = FGuid(Json->GetStringField("id"));

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Load mesh
	FString DefinitionName = FPaths::ProjectDir() + Json->GetStringField(L"type") + FString(".json");
	definition = JsonUtil::ReadFile(DefinitionName);
	FString MeshPath = definition->GetStringField(L"mesh");

	FSoftObjectPath MethObjectPath(*MeshPath);
	UStaticMesh* Mesh = AssetLibrary::LoadAsset<UStaticMesh>(*MeshPath);
	if (Mesh != nullptr) {
		StaticMesh->SetStaticMesh(Mesh);

		StaticMesh->SetRelativeLocation(FVector(0.f));
		StaticMesh->SetRelativeRotation(FQuat());
		StaticMesh->SetRelativeScale3D(FVector(1.f));
	}

	// set locaiton and scale
	GetRootComponent()->SetRelativeLocation(GetRelativeLocation());
	GetRootComponent()->SetRelativeScale3D(JsonUtil::Vector(Json->GetArrayField(L"scale")));

	for (auto& node : definition->GetArrayField(L"attachment")) {
		auto location = JsonUtil::Vector(node->AsObject()->GetArrayField(L"location"));

		auto attachment_node = NewObject<UAttachmentNode>(this);
		attachment_node->Initialize(GetRootComponent(), location);

		AttachmentNodes.Add(attachment_node);
	}
}

void APart::SetAttachmentNodeVisibility(bool visibility) {
	for (auto node : AttachmentNodes) {
		node->SetVisibility(visibility);
	}
}

// Called when the game starts or when spawned
void APart::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APart::SetRelativeLocation(const FVector& NewLocation) {
	Json->SetArrayField(L"location", JsonUtil::Vector(NewLocation));
	SetActorLocation(NewLocation + GetOwner()->GetActorLocation());
}

FVector APart::GetRelativeLocation() {
	return JsonUtil::Vector(Json->GetArrayField("location"));
}

