// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataAssets/DA_DataListEntryMapping.h"
#include "Widgets/Options/DataObjects/ListDataObjectBase.h"


TSubclassOf<UListEntryBase> UDA_DataListEntryMapping::FindEntryClassByDataObject(const UListDataObjectBase* DataObject) const
{
	check(DataObject);

	for (UClass* DataObjectClass = DataObject->GetClass();DataObjectClass;DataObjectClass->GetSuperClass())
	{
		if (TSubclassOf<UListDataObjectBase> ConvertedDataObjectClass = TSubclassOf<UListDataObjectBase>(DataObjectClass))
		{
			if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}
	}

	return TSubclassOf<UListEntryBase>();
}
