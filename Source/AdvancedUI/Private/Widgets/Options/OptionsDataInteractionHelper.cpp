// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Settings/AdvancedGameUserSettings.h"

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& SetterOrGetterFuncPath)
	:CachedDynamicFunctionPath(SetterOrGetterFuncPath)
{
	CachedGameUserSettingsWeak = UAdvancedGameUserSettings::Get();
}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString ReturnString;
	PropertyPathHelpers::GetPropertyValueAsString(CachedGameUserSettingsWeak.Get(), CachedDynamicFunctionPath, ReturnString);
	return ReturnString;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& StringValue)
{
	PropertyPathHelpers::SetPropertyValueFromString(CachedGameUserSettingsWeak.Get(),CachedDynamicFunctionPath, StringValue);
}