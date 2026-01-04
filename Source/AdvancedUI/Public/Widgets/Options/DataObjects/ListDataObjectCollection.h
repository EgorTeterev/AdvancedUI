// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObjectBase.h"
#include "ListDataObjectCollection.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UListDataObjectCollection : public UListDataObjectBase
{
	GENERATED_BODY()

public:
	void AddChildList(UListDataObjectBase* ChildListDataToAdd);

	virtual TArray<UListDataObjectBase*> GetAllChildListData() const;
	virtual bool HasAnyChildListData() const;

private:
	UPROPERTY(Transient)
	TArray<UListDataObjectBase*> ChildListDataArray;


};
