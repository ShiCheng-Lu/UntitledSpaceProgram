// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SEditableTextBox.h"

/**
 * 
 */
class MYPROJECT_API SSliderWithInput : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSliderWithInput)
	{}
	SLATE_ARGUMENT(FString, Label)
	SLATE_ARGUMENT(double, Value)
	SLATE_EVENT(FOnFloatValueChanged, OnValueChanged)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	float Value;
	TSharedPtr<SHorizontalBox> Box;
	TSharedPtr<SSlider> Slider;
	TSharedPtr<SEditableTextBox> InputBox;
	FOnFloatValueChanged OnValueChanged;

	void OnValueChange(float InValue);
	void OnTextCommitted(const FText& InText, ETextCommit::Type commitType);
	void OnTextChanged(const FText& InText);
	FReply OnClickIncrement();
	FReply OnClickDecrement();
};
