// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class MYPROJECT_API SPartMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPartMenu)
	{} //
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void Rebuild();
protected:
	TSharedPtr<SVerticalBox> ListBox;
	TSharedPtr<FJsonObject> Data;
	TSharedPtr<FJsonObject> Definition;
};
