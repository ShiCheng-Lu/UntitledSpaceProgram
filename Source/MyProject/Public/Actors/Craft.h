// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Part.h"
#include "Components/SphereComponent.h"

#include "Craft.generated.h"

UCLASS()
class MYPROJECT_API ACraft : public AActor
{
	GENERATED_BODY()

public:
	TSharedPtr<FJsonObject> Json;
	TMap<FString, APart*> Parts;
	USphereComponent* Root;
	APart* RootPart;

public:
	ACraft();

	void Initialize(TSharedPtr<FJsonObject> CraftJson);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<APart*> CreatePartStructure(TSharedPtr<FJsonObject> StructureJson, APart* StructureParent);

	void AddPart(APart* Part);

	void RemovePart(APart* Part);

	static void TransferPart(APart* Part, ACraft* FromCraft, ACraft* ToCraft);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetAttachmentNodeVisibility(bool visibility);

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// void TransferPart(APart* Part, ACraft* OtherCraft);

	ACraft* DetachPart(APart* Part);

	void AttachPart(ACraft* OtherCraft, APart* AttachToPart);

	void UpdateJsonPartsArray();

	void SetActorLocation(const FVector& NewLocation);
};
