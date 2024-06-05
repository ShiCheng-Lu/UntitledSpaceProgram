// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SPartDetails.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class MYPROJECT_API SConstructionUI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SConstructionUI)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void ShowPart(TSharedPtr<FJsonObject> Part);

	void SelectPartToPlace(TSharedPtr<FJsonObject> Part);
private:
	TSharedPtr<SHorizontalBox> ListBox;
	TSharedPtr<SPartDetails> PartDetails;
};
