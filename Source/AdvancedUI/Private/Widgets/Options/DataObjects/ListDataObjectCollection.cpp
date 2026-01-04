// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObjectCollection.h"

void UListDataObjectCollection::AddChildList(UListDataObjectBase* ChildListDataToAdd)
{
	//Notify child to init itself
	ChildListDataToAdd->InitDataObject();

	//Set this to childs parent
	ChildListDataToAdd->SetParentData(this);

	ChildListDataArray.Add(ChildListDataToAdd);
}

TArray<UListDataObjectBase*> UListDataObjectCollection::GetAllChildListData() const
{
	return ChildListDataArray;
}

bool UListDataObjectCollection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}