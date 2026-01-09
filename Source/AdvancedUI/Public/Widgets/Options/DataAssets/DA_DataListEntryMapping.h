// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_DataListEntryMapping.generated.h"
class UListDataObjectBase;
class UListEntryBase;
/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UDA_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()
	

public:
	TSubclassOf<UListEntryBase> FindEntryClassByDataObject(const UListDataObjectBase* DataObject) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UListDataObjectBase>, TSubclassOf<UListEntryBase>> DataObjectListEntryMap;
};
