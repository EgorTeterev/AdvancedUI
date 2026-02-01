// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/ListEntryString.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonInputSubsystem.h"
#include "Widgets/Components/AdvancedCommonRotator.h"


void UListEntryString::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PreviousOptionCommonButton->OnClicked().AddUObject(this,&ThisClass::OnPreviousOptionButtonClicked);
	NextOptionCommonButton->OnClicked().AddUObject(this, &ThisClass::OnNextOptionButtonClicked);

	OptionCommonRotator->OnClicked().AddLambda([this]() {SelectThisEntryWidget();});
	OptionCommonRotator->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorValueChanged);
}

void UListEntryString::OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject)
{
	Super::OnOwningListDataObjectSet(OwningListDataObject);

	CachedStringDataObject = CastChecked<UListDataObject_String>(OwningListDataObject);

	if (CachedStringDataObject)
	{
		OptionCommonRotator->PopulateTextLabels(CachedStringDataObject->GetOptionsTextArray());
		OptionCommonRotator->SetSelectedOptionByText(CachedStringDataObject->GetCurrentDisplayText());
	}

}

void UListEntryString::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);

	PreviousOptionCommonButton->SetIsEnabled(bIsEditable);
	OptionCommonRotator->SetIsEnabled(bIsEditable);
	NextOptionCommonButton->SetIsEnabled(bIsEditable);
}

void UListEntryString::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedStringDataObject)
	{
		OptionCommonRotator->SetSelectedOptionByText(CachedStringDataObject->GetCurrentDisplayText());
	}
}

void UListEntryString::OnPreviousOptionButtonClicked()
{
	if (CachedStringDataObject)
	{
		CachedStringDataObject->SwitchToNextOption();
	}

	SelectThisEntryWidget();
}

void UListEntryString::OnNextOptionButtonClicked()
{
	if (CachedStringDataObject)
	{
		CachedStringDataObject->SwitchToPreviousOption();
	}

	SelectThisEntryWidget();
}

void UListEntryString::OnRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	if (CachedStringDataObject)
	{
		UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();

		if (CommonInputSubsystem && bUserInitiated)
		{
			if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
			{
				CachedStringDataObject->OnRotatorInitiatedChange(OptionCommonRotator->GetSelectedText());
			}
		}
	}
}
