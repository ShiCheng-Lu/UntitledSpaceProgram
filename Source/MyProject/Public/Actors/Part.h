// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Misc/Guid.h"
#include "JsonUtil.h"
#include "Actors/AttachmentNode.h"
#include "Components/StaticMeshComponent.h"
#include "UI/SConstructionUI.h"

#include "Part.generated.h"

UCLASS()
class MYPROJECT_API APart : public AActor
{
	GENERATED_BODY()

public:
	TSharedPtr<FJsonObject> Json;
	TSharedPtr<FJsonObject> Structure;
	FString Id;
	TSharedPtr<FJsonObject> definition;
	TArray<APart*> Children;
	APart* Parent;

public:
	APart();

	void Initialize(TSharedPtr<FJsonObject> Json);
	
	void SetAttachmentNodeVisibility(bool visibility);

	void SetParent(APart* Part);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetRelativeLocation(const FVector& NewLocation);
	FVector GetRelativeLocation();

	UStaticMeshComponent* StaticMesh;
	TArray<UAttachmentNode*> AttachmentNodes;
};
