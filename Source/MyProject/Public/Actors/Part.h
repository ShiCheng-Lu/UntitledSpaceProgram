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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UPart : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	TSharedPtr<FJsonObject> Json;
	TSharedPtr<FJsonObject> Structure;

	TSharedPtr<FJsonObject> definition;

	FString Id;
	TArray<UPart*> Children;
	UPart* Parent;

	TArray<UAttachmentNode*> AttachmentNodes;

	UPart();

	void Initialize(USceneComponent* InParent, TSharedPtr<FJsonObject> Json);
	
	void SetAttachmentNodeVisibility(bool visibility);

	void SetParent(UPart* Part);
};
