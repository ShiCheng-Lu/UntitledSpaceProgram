// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Part.h"

#include "Craft.generated.h"

UCLASS()
class MYPROJECT_API ACraft : public AActor
{
	GENERATED_BODY()

public:
	TSharedPtr<FJsonObject> Json;

public:
	void Initialize(TSharedPtr<FJsonObject> CraftJson);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetAttachmentNodeVisibility(bool visibility);

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TMap<FGuid, APart*> Parts;
};
