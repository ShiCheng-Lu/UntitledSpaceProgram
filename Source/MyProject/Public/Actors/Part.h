// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Misc/Guid.h"
#include "JsonUtil.h"
#include "Components/StaticMeshComponent.h"
#include "UI/SConstructionUI.h"

#include "Part.generated.h"

UCLASS()
class MYPROJECT_API APart : public AActor
{
	GENERATED_BODY()

public:
	TSharedPtr<FJsonObject> Json;
	FGuid Id;
	TSharedPtr<FJsonObject> definition;

public:
	APart();

	void Initialize(TSharedPtr<FJsonObject> Json);

	void Attach(TMap<FGuid, TSharedPtr<APart>> PartList);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* StaticMesh;
};
