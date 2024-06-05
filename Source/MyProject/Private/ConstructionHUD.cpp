// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionHUD.h"
#include "UI/SPartMenu.h"
#include "JsonUtil.h"
#include "UI/SConstructionUI.h"
#include "Actors/Craft.h"

AConstructionHUD::AConstructionHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	//

}

void AConstructionHUD::BeginPlay() {
	Super::BeginPlay();

	MyWidget = SNew(SConstructionUI);

	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	ViewportClient->AddViewportWidgetContent(MyWidget.ToSharedRef());

	ACraft* Craft = GetWorld()->SpawnActor<ACraft>();
	Craft->Initialize(JsonUtil::ReadFile(FPaths::ProjectDir() + "ship.json"));
}
