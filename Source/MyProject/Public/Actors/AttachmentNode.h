// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "AttachmentNode.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UAttachmentNode : public USphereComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttachmentNode(const FObjectInitializer& ObjectInitializer);

	void Initialize(USceneComponent *InParent, FVector& InRelativeLocation);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UStaticMeshComponent* Mesh;
	FVector RelativeLocation;
};
