// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"


FCommonNumberFormattingOptions UListDataObject_Scalar::NoDecimal()
{
    FCommonNumberFormattingOptions Options;
    Options.MaximumFractionalDigits = 0;
    return Options;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::WithDecimal(int32 NumFracDigit)
{
    FCommonNumberFormattingOptions Options;
    Options.MaximumFractionalDigits = NumFracDigit;
    return Options;
}

float UListDataObject_Scalar::GetCurrentValue() const
{
    if (DataDynamicGetter)
    { 
        return FMath::GetMappedRangeValueClamped(
            OutputValueRange,
            DisplayValueRange,
            StringToFloat(DataDynamicGetter->GetValueAsString())
        );
    }
    return 0.0f;
}

void UListDataObject_Scalar::SetCurrentValueFromSlider(float NewValue)
{
    if (DataDynamicSetter)
    {
        const float ClampedValue = FMath::GetMappedRangeValueClamped(
            DisplayValueRange,
            OutputValueRange,
            NewValue
        );

        DataDynamicSetter->SetValueFromString(LexToSanitizedString(ClampedValue));

        NotifyListDataModified(this);
    }
}

float UListDataObject_Scalar::StringToFloat(const FString& String) const
{
    float OutValue = 0.0f;
    LexFromString(OutValue, *String);
    return OutValue;
}
bool UListDataObject_Scalar::CanResetBackToDefaultValue() const
{
    if (HasDefaultValue())
    {
       const float DefaultValue = StringToFloat(GetDefaultValueAsString());
       const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());

       return !FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.01f);
    }

    return false;
}

bool UListDataObject_Scalar::TryResetBackToDefaultValue()
{
    if (CanResetBackToDefaultValue())
    {
        if (DataDynamicSetter)
        {
            DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());

            NotifyListDataModified(this,EOptionsListDataModifyReason::ResetToDefault);

            return true;
        }
    }

    return false;
}