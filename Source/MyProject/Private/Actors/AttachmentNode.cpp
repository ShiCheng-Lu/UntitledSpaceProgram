// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/AttachmentNode.h"
#include "AssetLibrary.h"

// Sets default values for this component's properties
UAttachmentNode::UAttachmentNode(const FObjectInitializer& ObjectInitializer) : USphereComponent(ObjectInitializer)
{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("/Game/Shapes/Shape_Sphere.Shape_Sphere"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	Mesh->SetupAttachment(this);
	Mesh->SetStaticMesh(SphereMeshAsset.Object);
	Mesh->SetRelativeLocation(FVector(0.f));
	Mesh->SetRelativeScale3D(FVector(0.1f));
}

void UAttachmentNode::Initialize(USceneComponent* InParent, FVector& InRelativeLocation) {
	RelativeLocation = InRelativeLocation;

	SetupAttachment(InParent);
	SetRelativeLocation(RelativeLocation);
	SetSphereRadius(20.f);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetGenerateOverlapEvents(true);

	RegisterComponent();
	Mesh->RegisterComponent();
}

// Called when the game starts
void UAttachmentNode::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UAttachmentNode::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
