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
	FGuid Id;
	TSharedPtr<FJsonObject> definition;
	TArray<APart*> Children;

public:
	APart();

	void Initialize(TSharedPtr<FJsonObject> Json);
	
	void SetAttachmentNodeVisibility(bool visibility);

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
