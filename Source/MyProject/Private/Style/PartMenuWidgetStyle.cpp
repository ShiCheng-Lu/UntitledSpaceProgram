// Fill out your copyright notice in the Description page of Project Settings.


#include "Style/PartMenuWidgetStyle.h"

FPartMenuStyle::FPartMenuStyle()
{
}

FPartMenuStyle::~FPartMenuStyle()
{
}

const FName FPartMenuStyle::TypeName(TEXT("FPartMenuStyle"));

const FPartMenuStyle& FPartMenuStyle::GetDefault()
{
	static FPartMenuStyle Default;
	return Default;
}

void FPartMenuStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

