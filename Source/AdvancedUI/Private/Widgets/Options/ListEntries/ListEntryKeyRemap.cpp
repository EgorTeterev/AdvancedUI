// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/ListEntryKeyRemap.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

void UListEntryKeyRemap::OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject)
{
	Super::OnOwningListDataObjectSet(OwningListDataObject);
	CachedRemapObject = CastChecked<UListDataObject_KeyRemap>(OwningListDataObject);

	CommonButtonKeyRemap->SetButtonDisplayImage(CachedRemapObject->GetIconFromCurrentKey());
}

void UListEntryKeyRemap::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedRemapObject)
	{
		CommonButtonKeyRemap->SetButtonDisplayImage(CachedRemapObject->GetIconFromCurrentKey());
	}
}