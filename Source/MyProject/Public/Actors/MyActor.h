// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonUtil.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "UI/SConstructionUI.h"
#include "MyActor.generated.h"


// we want:
// Part class
// Vehicle class (collection of parts and other defs)
// 

UCLASS()
class MYPROJECT_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();
	void AttachUI(TSharedPtr<SConstructionUI> ui);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

	TSharedPtr<AMyActor> Parent;
	TArray<TSharedPtr<AMyActor>> Children;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* StaticMesh;
	TSharedPtr<FJsonObject> Part;
	FVector3d Size;
	TSharedPtr<SConstructionUI> ui;
};
