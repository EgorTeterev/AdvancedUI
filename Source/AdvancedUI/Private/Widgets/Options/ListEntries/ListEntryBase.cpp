// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/ListEntryBase.h"
#include "Widgets/Options/DataObjects/ListDataObjectBase.h"
#include "CommonInputSubsystem.h"
#include "Components/ListView.h"
#include "CommonTextBlock.h"



void UListEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	SetVisibility(ESlateVisibility::Visible);

	OnOwningListDataObjectSet(CastChecked<UListDataObjectBase>(ListItemObject));

}

FReply UListEntryBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UCommonInputSubsystem* InputSubsystem = GetInputSubsystem();

	if (InputSubsystem && InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* WidgetToFocus = BP_GetWidgetToFocusForGamepad())
		{
			if (TSharedPtr<SWidget> SlateWidgetToFocus = WidgetToFocus->GetCachedWidget())
			{
				return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef(), EFocusCause::SetDirectly);
			}
		}
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UListEntryBase::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, IsListItemSelected());
}

void UListEntryBase::OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject)
{
	if (CommonTextDisplayName)
	{
		CommonTextDisplayName->SetText(OwningListDataObject->GetDataDisplayName());
	}

	if (!OwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		OwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}
}

void UListEntryBase::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{

}

void UListEntryBase::SelectThisEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}
