// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Types/AdvancedUIEnumTypes.h"
#include "Types/AdvancedUIStructTypes.h"
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
	FOnListDataModifiedDelegate OnDependecyObjectModified;

	LIST_DATA_ACCESSORS(FName, DataID);
	LIST_DATA_ACCESSORS(FText, DataDisplayName);
	LIST_DATA_ACCESSORS(FText, DescriptionRichText);
	LIST_DATA_ACCESSORS(FText, DisabledRichText);
	LIST_DATA_ACCESSORS(TSoftObjectPtr<UTexture2D>, SoftDescriprionImage);
	LIST_DATA_ACCESSORS(UListDataObjectBase*, ParentData);

	virtual bool HasDefaultValue() const { return false; };
	virtual bool CanResetBackToDefaultValue() const { return false; };
	virtual bool TryResetBackToDefaultValue() { return false; };

	virtual TArray<UListDataObjectBase*> GetAllChildListData() const { return TArray<UListDataObjectBase*>(); };
	virtual bool HasAnyChildListData() const { return false; };
	void SetShouldApplyChangeSettingsImmediatly(bool bShouldApply) { bShouldApplyChangeSettingsImmediatly = bShouldApply; };
	
	void AddEditCondition(const FOptionDataEditConditionDescriptor& EditCondition);
	bool IsDataCurrenlyEditable();
	
	void AddEditDependencyObject(UListDataObjectBase* ObjectToDependOn);
protected:
	virtual void OnDataObjectInitialized();
	virtual void NotifyListDataModified(UListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified);
	
	//override to allow value to be set to the forced string value
	virtual bool CanSetToForcedStringValue(const FString& ForcedValue) const { return false; }

	//override to specify how to set the current value to forced value
	virtual void OnSetToForcedStringValue(const FString& ForcedValue) {}

	void OnDependencyObjectWasModified(UListDataObjectBase* ModifiedDependencyObject, EOptionsListDataModifyReason ModifyReason);

private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriprionImage;

	UPROPERTY(Transient)
	UListDataObjectBase* ParentData;

	bool bShouldApplyChangeSettingsImmediatly = false;

	UPROPERTY(Transient)
	TArray<FOptionDataEditConditionDescriptor> EditConditionArray;
};
