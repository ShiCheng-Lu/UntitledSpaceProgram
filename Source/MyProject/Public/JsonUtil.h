// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MYPROJECT_API JsonUtil
{
public:
	static TSharedPtr<FJsonObject> ReadFile(FString FilePath);
	static bool WriteFile(FString FilePath, TSharedPtr<FJsonObject> JsonObject);

	static FVector Vector(const TArray<TSharedPtr<FJsonValue>>& Values);
	static TArray<TSharedPtr<FJsonValue>> Vector(const FVector& Vector);
};
