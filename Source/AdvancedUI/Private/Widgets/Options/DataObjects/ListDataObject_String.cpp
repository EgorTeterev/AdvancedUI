// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"


void UListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}

	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Invalid option"));
	}

}

void UListDataObject_String::AddDynamicOption(const FString& AddOptionString, const FText& AddDisplayText)
{
	AvailableOptionsStringArray.Add(AddOptionString);
	AvailableOptionsTextArray.Add(AddDisplayText);
}

void UListDataObject_String::SwitchToNextOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 NextIndexToDisplay = CurrentDisplayIndex + 1;

	if (AvailableOptionsStringArray.IsValidIndex(NextIndexToDisplay))
	{
		CurrentStringValue = AvailableOptionsStringArray[NextIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this);
	}
}

void UListDataObject_String::SwitchToPreviousOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PreviousIndexToDisplay = CurrentDisplayIndex -1;

	if (AvailableOptionsStringArray.IsValidIndex(PreviousIndexToDisplay))
	{
		CurrentStringValue = AvailableOptionsStringArray[PreviousIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray.Last();
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this);
	}
}

void UListDataObject_String::OnRotatorInitiatedChange(const FText& NewSelectedText)
{
	const int32 FoundIndex = AvailableOptionsTextArray.IndexOfByPredicate(
		[NewSelectedText](const FText& AvailableText) -> bool
		{
			return AvailableText.EqualTo(NewSelectedText);
		});

	if (FoundIndex != NULL && AvailableOptionsStringArray.IsValidIndex(FoundIndex))
	{
		CurrentDisplayText = NewSelectedText;
		CurrentStringValue = AvailableOptionsStringArray[FoundIndex];

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);

			NotifyListDataModified(this);
		}
	}
}

bool UListDataObject_String::TrySetDisplayTextFromStringValue(const FString& String)
{
	const int32 Index = AvailableOptionsStringArray.IndexOfByKey(String);

	if (AvailableOptionsTextArray.IsValidIndex(Index))
	{
		CurrentDisplayText = AvailableOptionsTextArray[Index];
		return true;
	}

	return false;
}

bool UListDataObject_String::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UListDataObject_String::TryResetBackToDefaultValue() 
{
	if (CanResetBackToDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
		TrySetDisplayTextFromStringValue(CurrentStringValue);

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
			NotifyListDataModified(this, EOptionsListDataModifyReason::ResetToDefault);

			return true;
		}
	}
	return false;
}