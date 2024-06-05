// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SSliderWithInput.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSliderWithInput::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SAssignNew(Box, SHorizontalBox)
		];

	Value = InArgs._Value;
	OnValueChanged = InArgs._OnValueChanged;

	Box->AddSlot()
		.FillWidth(0.3)
		.HAlign(EHorizontalAlignment::HAlign_Right)
		.Padding(5)
		[
			SNew(STextBlock).Text(FText::FromString(InArgs._Label))
		];

	Box->AddSlot()
		.FillWidth(0.1)
		.Padding(5)
		[
			SNew(SButton).Text(NSLOCTEXT("-", "-", "-"))
			.OnClicked(this, &SSliderWithInput::OnClickDecrement)
		];

	Box->AddSlot()
		.FillWidth(0.5)
		.Padding(5)
		[
			SAssignNew(Slider, SSlider)
			.OnValueChanged(this, &SSliderWithInput::OnValueChange)
			.MinValue(1)
			.MaxValue(10)
			.MouseUsesStep(true)
			.StepSize(0.25)
			.Value(InArgs._Value)
		];
	
	Box->AddSlot()
		.FillWidth(0.1)
		.Padding(5)
		[
			SNew(SButton).Text(NSLOCTEXT("+", "+", "+"))
			.OnClicked(this, &SSliderWithInput::OnClickIncrement)
		];

	Box->AddSlot()
		.FillWidth(0.3)
		.Padding(5)
		[
			SAssignNew(InputBox, SEditableTextBox)
			.OnTextCommitted(this, &SSliderWithInput::OnTextCommitted)
			.Text(FText::FromString(FString::SanitizeFloat(InArgs._Value)))
		];
}

void SSliderWithInput::OnValueChange(float InValue) {
	Value = fmax(fmin(InValue, 500), 0.25);
	Slider->SetValue(InValue);
	InputBox->SetText(FText::FromString(FString::SanitizeFloat(InValue)));

	OnValueChanged.Execute(Value);
}

void SSliderWithInput::OnTextCommitted(const FText& InText, ETextCommit::Type commitType) {
	try {
		float value = FCString::Atof(*InText.ToString());
		this->OnValueChange(value);
	}
	catch (...) {
		this->OnValueChange(Value);
	}
}

FReply SSliderWithInput::OnClickIncrement() {
	this->OnValueChange(Value + 0.25);

	return FReply::Handled();
}

FReply SSliderWithInput::OnClickDecrement() {
	this->OnValueChange(Value - 0.25);

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
