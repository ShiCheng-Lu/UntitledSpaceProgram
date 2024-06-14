// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/AttachmentNode.h"
#include "AssetLibrary.h"

// Sets default values for this component's properties
UAttachmentNode::UAttachmentNode(const FObjectInitializer& ObjectInitializer) : UStaticMeshComponent(ObjectInitializer)
{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("/Game/Shapes/Shape_Sphere.Shape_Sphere"));

	SetStaticMesh(SphereMeshAsset.Object);
	SetRelativeScale3D(FVector(0.1f));
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UAttachmentNode::Initialize(USceneComponent* InParent, FVector& InRelativeLocation) {
	RelativeLocation = InRelativeLocation;

	SetupAttachment(InParent);
	SetRelativeLocation(RelativeLocation);

	RegisterComponent();
}
