// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UAdvancedGameUserSettings;

/**
 * 
 */
class ADVANCEDUI_API FOptionsDataInteractionHelper
{
public:
	FOptionsDataInteractionHelper(const FString& SetterOrGetterFuncPath);

	FString GetValueAsString() const;
	void SetValueFromString(const FString& StringValue);

private:
	FCachedPropertyPath CachedDynamicFunctionPath;
	TWeakObjectPtr<UAdvancedGameUserSettings> CachedGameUserSettingsWeak;
};
