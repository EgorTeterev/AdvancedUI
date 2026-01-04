// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ListDataObjectBase.generated.h"

#define LIST_DATA_ACCESSORS(DataType,PropertyName) \
FORCEINLINE DataType Get##PropertyName() const {return PropertyName;}; \
void Set##PropertyName (DataType PropertyName##ToSet) {PropertyName = PropertyName##ToSet;};

/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UListDataObjectBase : public UObject
{
	GENERATED_BODY()
	
public:
	LIST_DATA_ACCESSORS(FName, DataId);
	LIST_DATA_ACCESSORS(FText, DataDisplayName);
	LIST_DATA_ACCESSORS(FText, DescriptionRichText);
	LIST_DATA_ACCESSORS(FText, DisabledRichText);
	LIST_DATA_ACCESSORS(TSoftObjectPtr<UTexture2D>, SoftDescriprionImage);
	LIST_DATA_ACCESSORS(UListDataObjectBase*, ParentData);

	void InitDataObject();

	virtual TArray<UListDataObjectBase*> GetAllChildListData() const { return TArray<UListDataObjectBase*>(); };
	virtual bool HasAnyChildListData() const { return false; };
protected:

	virtual void OnDataObjectInitialized();

private:
	FName DataId;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriprionImage;

	UPROPERTY(Transient)
	UListDataObjectBase* ParentData;
};
