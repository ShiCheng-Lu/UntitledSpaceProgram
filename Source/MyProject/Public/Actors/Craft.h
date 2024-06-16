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
	TMap<FString, UPart*> Parts;
	UPart* RootPart;
	UPart* Engine;

public:
	ACraft();

	void Initialize(TSharedPtr<FJsonObject> CraftJson);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<UPart*> CreatePartStructure(TSharedPtr<FJsonObject> StructureJson, UPart* StructureParent);

	void AddPart(UPart* Part);

	void RemovePart(UPart* Part);

	static void TransferPart(UPart* Part, ACraft* FromCraft, ACraft* ToCraft);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetAttachmentNodeVisibility(bool visibility);

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// void TransferPart(APart* Part, ACraft* OtherCraft);

	ACraft* DetachPart(UPart* Part);

	void AttachPart(ACraft* OtherCraft, UPart* AttachToPart);

	void UpdateJsonPartsArray();

	void Throttle(float throttle);
};
