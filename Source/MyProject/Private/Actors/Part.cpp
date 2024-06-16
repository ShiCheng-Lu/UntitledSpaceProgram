// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Part.h"
#include "Engine/StreamableManager.h"
#include "ConstructionMode.h"
#include "AssetLibrary.h"
#include "Engine/StaticMesh.h"
#include "Actors/AttachmentNode.h"

UPart::UPart(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SetCollisionEnabled(ECollisionEnabled::QueryAndProbe); // QueryAndProbe for constructor, QueryAndPhysics for simulation
	Parent = nullptr;
	Children = TArray<UPart*>();

	// SetSimulatePhysics(true);

	Physics = CreateDefaultSubobject<UPhysicsConstraintComponent>("Link");
	// Physics->SetupAttachment(this);
	Physics->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	Physics->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
	Physics->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
}

// Sets default values
void UPart::Initialize(FString InId, TSharedPtr<FJsonObject> InStructure, TSharedPtr<FJsonObject> InJson)
{
	Id = InId;
	Structure = InStructure;
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
}

void UPart::SetAttachmentNodeVisibility(bool visibility) {
	for (auto node : AttachmentNodes) {
		node->SetVisibility(visibility);
	}
}

void UPart::SetParent(UPart* NewParent) {
	if (Parent != nullptr) {
		Parent->Children.Remove(this);
		Parent->Structure->RemoveField(Id);
	}
	Parent = NewParent;
	if (Parent != nullptr) {
		Parent->Children.Add(this);
		if (!Parent->Structure->HasField(Id)) {
			Parent->Structure->SetObjectField(Id, Structure);
		}
		AttachToComponent(Parent, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
		// Physics->SetConstrainedComponents(this, "", Parent, "");
	}
	else {
		DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		// Physics->BreakConstraint();
	}
}
