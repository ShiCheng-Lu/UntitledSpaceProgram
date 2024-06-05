// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
    StaticMesh->SetupAttachment(GetRootComponent());
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderAsset(TEXT("/Game/Shapes/Shape_Cylinder.Shape_Cylinder"));


    Part = JsonUtil::ReadFile(FPaths::ProjectDir() + TEXT("part.json"));

    Size = FVector(1.0f);
    if (CylinderAsset.Succeeded())
    {
        StaticMesh->SetStaticMesh(CylinderAsset.Object);
        StaticMesh->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f));
        StaticMesh->SetWorldScale3D(Size);
    }
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    Size.Z = Part->GetNumberField(L"height");
    Size.X = Part->GetNumberField(L"width");
    Size.Y = Part->GetNumberField(L"width");

    StaticMesh->SetWorldScale3D(Size);
}

void AMyActor::AttachUI(TSharedPtr<SConstructionUI> InUI) {
    this->ui = InUI;
}

void AMyActor::NotifyActorOnClicked(FKey ButtonPressed) {
    if (ButtonPressed == FKey("RightMouseButton")) {
        UE_LOG(LogTemp, Warning, TEXT("Clicked %s"), *ButtonPressed.GetFName().ToString());
        // ui->ShowPart(Part);
    }
    else if (ButtonPressed == FKey("LeftMouseButton")) {
        UE_LOG(LogTemp, Warning, TEXT("Clicked %s"), *ButtonPressed.GetFName().ToString());
    }
}

