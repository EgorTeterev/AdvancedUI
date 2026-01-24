// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObjectBase.h"
#include "ListDataObject_Value.generated.h"

class FOptionsDataInteractionHelper;
/**
 * 
 */
UCLASS(Abstract)
class ADVANCEDUI_API UListDataObject_Value : public UListDataObjectBase
{
	GENERATED_BODY()

public:
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper> OtherDynamicSetter);
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper> OtherDynamicGetter);

	void SetDefaultValueFromString(const FString& DefaultValue) { DefaultStringValue = DefaultValue; };
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); };

protected:
	FString GetDefaultValueAsString() const { return DefaultStringValue.GetValue(); };

	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;

private:
	TOptional<FString> DefaultStringValue;
};