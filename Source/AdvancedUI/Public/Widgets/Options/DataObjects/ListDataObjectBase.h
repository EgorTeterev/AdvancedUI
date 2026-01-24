// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Types/AdvancedUIEnumTypes.h"
#include "ListDataObjectBase.generated.h"

#define LIST_DATA_ACCESSORS(DataType,PropertyName) \
FORCEINLINE DataType Get##PropertyName() const {return PropertyName;}; \
void Set##PropertyName (DataType PropertyName##ToSet) {PropertyName = PropertyName##ToSet;};

/**
 * 
 */
UCLASS(Abstract)
class ADVANCEDUI_API UListDataObjectBase : public UObject
{
	GENERATED_BODY()
	
public:
	void InitDataObject();
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UListDataObjectBase*, EOptionsListDataModifyReason);
	FOnListDataModifiedDelegate OnListDataModified;

	LIST_DATA_ACCESSORS(FName, DataID);
	LIST_DATA_ACCESSORS(FText, DataDisplayName);
	LIST_DATA_ACCESSORS(FText, DescriptionRichText);
	LIST_DATA_ACCESSORS(FText, DisabledRichText);
	LIST_DATA_ACCESSORS(TSoftObjectPtr<UTexture2D>, SoftDescriprionImage);
	LIST_DATA_ACCESSORS(UListDataObjectBase*, ParentData);

	virtual bool HasDefaultValue() const { return false; };
	virtual bool CanResetBackToDefaultValue() const { return false; };
	virtual bool TryResetBackToDefaultValue() const { return false; };

	virtual TArray<UListDataObjectBase*> GetAllChildListData() const { return TArray<UListDataObjectBase*>(); };
	virtual bool HasAnyChildListData() const { return false; };
	void SetShouldApplyChangeSettingsImmediatly(bool bShouldApply) { bShouldApplyChangeSettingsImmediatly = bShouldApply; };

protected:
	virtual void OnDataObjectInitialized();
	virtual void NotifyListDataModified(UListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);

private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriprionImage;

	UPROPERTY(Transient)
	UListDataObjectBase* ParentData;

	bool bShouldApplyChangeSettingsImmediatly = false;
};
