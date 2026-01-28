// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/ListEntryScalar.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "AnalogSlider.h"


void UListEntryScalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SettingSlider->OnValueChanged.AddUniqueDynamic(this,&ThisClass::OnSliderValueChanged);
	SettingSlider->OnMouseCaptureBegin.AddUniqueDynamic(this,&ThisClass::OnSliderMouseCaptureBegin);
}

void UListEntryScalar::OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject)
{
	Super::OnOwningListDataObjectSet(OwningListDataObject);

	CachedScalarDataObject = CastChecked<UListDataObject_Scalar>(OwningListDataObject);

	CommonNumericSettingValue->SetNumericType(CachedScalarDataObject->GetDisplayNumericType());
	CommonNumericSettingValue->FormattingSpecification = CachedScalarDataObject->GetNumberFormattingOptions();
	CommonNumericSettingValue->SetCurrentValue(CachedScalarDataObject->GetCurrentValue());

	SettingSlider->SetMinValue(CachedScalarDataObject->GetDisplayValueRange().GetLowerBoundValue());
	SettingSlider->SetMaxValue(CachedScalarDataObject->GetDisplayValueRange().GetUpperBoundValue());
	SettingSlider->SetStepSize(CachedScalarDataObject->GetSliderStep());
	SettingSlider->SetValue(CachedScalarDataObject->GetCurrentValue());


}

void UListEntryScalar::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedScalarDataObject)
	{
		CommonNumericSettingValue->SetCurrentValue(CachedScalarDataObject->GetCurrentValue());
		SettingSlider->SetValue(CachedScalarDataObject->GetCurrentValue());
	}
}

void UListEntryScalar::OnSliderValueChanged(float Value)
{
	if (CachedScalarDataObject)
	{
		CachedScalarDataObject->SetCurrentValueFromSlider(Value);
	}
}

void UListEntryScalar::OnSliderMouseCaptureBegin()
{
	SelectThisEntryWidget();
}
