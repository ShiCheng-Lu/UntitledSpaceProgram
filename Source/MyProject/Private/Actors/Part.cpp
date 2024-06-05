// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Part.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/StreamableManager.h"

APart::APart() {
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    // StaticMesh->SetupAttachment(GetRootComponent());
    SetRootComponent(StaticMesh);
}

// Sets default values
void APart::Initialize(TSharedPtr<FJsonObject> InJson)
{
    Json = InJson;
    // Id = FGuid(Json->GetStringField(L"id"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



    // Load mesh
    FString DefinitionName = FPaths::ProjectDir() + Json->GetStringField(L"type") + FString(".json");
    definition = JsonUtil::ReadFile(DefinitionName);
    FString MeshPath = definition->GetStringField(L"mesh");

    FSoftObjectPath MethObjectPath(*MeshPath);
    UStaticMesh* Mesh = Cast<UStaticMesh>(MethObjectPath.ResolveObject());
    if (Mesh == nullptr)
    {
        Mesh = CastChecked<UStaticMesh>(MethObjectPath.TryLoad());
    }
    if (Mesh == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Staic mesh failed"));
    }
    else {
        StaticMesh->SetStaticMesh(Mesh);
        StaticMesh->SetRelativeLocation(FVector(0.f));
        StaticMesh->SetRelativeScale3D(FVector(1.f));
    }

    // set locaiton and scale
    GetRootComponent()->SetRelativeLocation(JsonUtil::Vector(Json->GetArrayField(L"location")));
    GetRootComponent()->SetRelativeScale3D(JsonUtil::Vector(Json->GetArrayField(L"scale")));

}

void APart::Attach(TMap<FGuid, TSharedPtr<APart>> PartList) {

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

    if (Json.IsValid()) {
        // GetRootComponent()->SetRelativeScale3D(JsonUtil::Vector(Json->GetArrayField(L"scale")));
    }
}

