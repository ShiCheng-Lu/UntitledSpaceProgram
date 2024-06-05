// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SConstructionUI.h"
#include "GameFramework/HUD.h"
#include "ConstructionHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AConstructionHUD : public AHUD
{
	GENERATED_BODY()

public:
	AConstructionHUD(const FObjectInitializer& ObjectInitializer);

	TSharedPtr<SConstructionUI> MyWidget;
protected:
	virtual void BeginPlay() override;

};
