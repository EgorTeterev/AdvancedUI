// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/ListEntryString.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Components/AdvancedCommonRotator.h"


void UListEntryString::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PreviousOptionCommonButton->OnClicked().AddUObject(this,&ThisClass::OnPreviousOptionButtonClicked);
	NextOptionCommonButton->OnClicked().AddUObject(this, &ThisClass::OnNextOptionButtonClicked);
}

void UListEntryString::OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject)
{
	Super::OnOwningListDataObjectSet(OwningListDataObject);

	CachedDataObject = CastChecked<UListDataObject_String>(OwningListDataObject);

	if (CachedDataObject)
	{
		OptionCommonRotator->PopulateTextLabels(CachedDataObject->GetOptionsTextArray());
		OptionCommonRotator->SetSelectedOptionByText(CachedDataObject->GetCurrentDisplayText());
	}

}


void UListEntryString::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedDataObject)
	{
		OptionCommonRotator->SetSelectedOptionByText(CachedDataObject->GetCurrentDisplayText());
	}
}


void UListEntryString::OnPreviousOptionButtonClicked()
{
	if (CachedDataObject)
	{
		CachedDataObject->SwitchToNextOption();
	}
}

void UListEntryString::OnNextOptionButtonClicked()
{
	if (CachedDataObject)
	{
		CachedDataObject->SwitchToPreviousOption();
	}
}
