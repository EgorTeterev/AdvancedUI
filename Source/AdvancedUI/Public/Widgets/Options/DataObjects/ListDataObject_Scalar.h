// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"
#include "CommonNumericTextBlock.h"
#include "ListDataObject_Scalar.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UListDataObject_Scalar : public UListDataObject_Value
{
	GENERATED_BODY()
public:
	LIST_DATA_ACCESSORS(TRange<float>, DisplayValueRange);
	LIST_DATA_ACCESSORS(TRange<float>, OutputValueRange);
	LIST_DATA_ACCESSORS(ECommonNumericType, DisplayNumericType);
	LIST_DATA_ACCESSORS(FCommonNumberFormattingOptions, NumberFormattingOptions);
	LIST_DATA_ACCESSORS(float, SliderStep); 
	static FCommonNumberFormattingOptions NoDecimal();
	static FCommonNumberFormattingOptions WithDecimal(int32 NumFracDigit);
	
	float GetCurrentValue() const;
	void SetCurrentValueFromSlider(float NewValue);


private:
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

	float StringToFloat(const FString& String) const;
	TRange<float> DisplayValueRange = TRange<float>(0.f,1.f);
	TRange<float> OutputValueRange = TRange<float>(0.f, 1.f);
	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;
	FCommonNumberFormattingOptions NumberFormattingOptions;
	float SliderStep = 0.1f;
	
};
