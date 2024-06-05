// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SConstructionUI.h"
#include "SlateOptMacros.h"
#include "UI/SPartMenu.h"
#include "JsonUtil.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SConstructionUI::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SAssignNew(ListBox, SHorizontalBox)
		];

	ListBox->AddSlot().MaxWidth(500)
		[
			SNew(SPartMenu)
		];
	ListBox->AddSlot().MaxWidth(500)
		[
			SAssignNew(PartDetails, SPartDetails)
		];
}

void SConstructionUI::ShowPart(TSharedPtr<FJsonObject> Part) {
	if (!Part.IsValid()) {
		return;
	}

	PartDetails->SetPart(Part);
}

void SConstructionUI::SelectPartToPlace(TSharedPtr<FJsonObject> Part) {
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
