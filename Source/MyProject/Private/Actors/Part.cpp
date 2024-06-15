// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Part.h"
#include "Engine/StreamableManager.h"
#include "AssetLibrary.h"
#include "Engine/StaticMesh.h"
#include "Actors/AttachmentNode.h"

UPart::UPart() {
	SetSimulatePhysics(true);
}

// Sets default values
void UPart::Initialize(USceneComponent* InParent, TSharedPtr<FJsonObject> InJson)
{
	Json = InJson;
	
	// Load mesh
	FString DefinitionName = FPaths::ProjectDir() + Json->GetStringField(L"type") + FString(".json");
	definition = JsonUtil::ReadFile(DefinitionName);
	FString MeshPath = definition->GetStringField(L"mesh");

	FSoftObjectPath MethObjectPath(*MeshPath);
	UStaticMesh* Mesh = AssetLibrary::LoadAsset<UStaticMesh>(*MeshPath);
	if (Mesh != nullptr) {
		SetStaticMesh(Mesh);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("failed to load mesh for: %s"), *Id);
	}

	// set locaiton and scale
	SetRelativeLocation(JsonUtil::Vector(Json->GetArrayField(L"location")));
	SetRelativeScale3D(JsonUtil::Vector(Json->GetArrayField(L"scale")));
	
	for (auto& node : definition->GetArrayField(L"attachment")) {
		auto location = JsonUtil::Vector(node->AsObject()->GetArrayField(L"location"));

		auto attachment_node = NewObject<UAttachmentNode>(this);
		attachment_node->Initialize(this, location);

		AttachmentNodes.Add(attachment_node);
	}

	RegisterComponent();

	AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
}

void UPart::SetAttachmentNodeVisibility(bool visibility) {
	for (auto node : AttachmentNodes) {
		node->SetVisibility(visibility);
	}
}

void UPart::SetParent(UPart* NewParent) {
	if (Parent) {
		Parent->Children.Remove(this);
		Parent->Structure->RemoveField(Id);
	}
	if (NewParent) {
		Parent = NewParent;
		Parent->Children.Add(this);
		Parent->Structure->SetObjectField(Id, Structure);
	}
}
