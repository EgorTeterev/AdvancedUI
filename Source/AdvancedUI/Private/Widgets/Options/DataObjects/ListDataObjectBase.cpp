// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObjectBase.h"

void UListDataObjectBase::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObjectBase::OnDataObjectInitialized()
{
}

void UListDataObjectBase::NotifyListDataModified(UListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);
}
