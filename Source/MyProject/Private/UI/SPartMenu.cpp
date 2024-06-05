// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPartMenu.h"
#include "SlateOptMacros.h"
#include "UI/SSliderWithInput.h"
#include "Components/SlateWrapperTypes.h"
#include <functional>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPartMenu::Construct(const FArguments& InArgs) {
	ChildSlot
		[
			SAssignNew(ListBox, SVerticalBox)
		];
	Rebuild();
}

void SPartMenu::Rebuild() {
	ListBox->ClearChildren();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
