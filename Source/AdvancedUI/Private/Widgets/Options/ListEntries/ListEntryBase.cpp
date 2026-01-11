// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/ListEntryBase.h"
#include "Widgets/Options/DataObjects/ListDataObjectBase.h"
#include "CommonTextBlock.h"


void UListEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UListDataObjectBase>(ListItemObject));

}

void UListEntryBase::OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject)
{
	if (CommonTextDisplayName)
	{
		CommonTextDisplayName->SetText(OwningListDataObject->GetDataDisplayName());
	}
}
