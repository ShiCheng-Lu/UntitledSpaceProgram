// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPartDetails.h"
#include "SlateOptMacros.h"
#include "JsonUtil.h"
#include "UI/SSliderWithInput.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPartDetails::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SAssignNew(ListBox, SVerticalBox)
		];
}

void SPartDetails::SetPart(TSharedPtr<FJsonObject> Part) {
	if (!Part.IsValid()) {
		return;
	}

	FString DefinitionName = Part->GetStringField(L"type");
	TSharedPtr<FJsonObject> Definition = JsonUtil::ReadFile(FPaths::ProjectDir() + DefinitionName);

	ListBox->ClearChildren();

	for (auto& Field : Definition->GetArrayField(L"fields")) {
		TSharedPtr<FJsonObject> FieldJson = Field->AsObject();

		FString FieldName = FieldJson->GetStringField(L"name");

		FOnFloatValueChanged valueChangedDelegate;
		valueChangedDelegate.BindLambda([Part, FieldName](float value) {
			Part->SetNumberField(FieldName, value);
		});

		ListBox->AddSlot().MaxHeight(40)
			[
				SNew(SSliderWithInput)
				.Label(FieldName)
			.Value(Part->GetNumberField(FieldName))
			.OnValueChanged(valueChangedDelegate)
			];
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
