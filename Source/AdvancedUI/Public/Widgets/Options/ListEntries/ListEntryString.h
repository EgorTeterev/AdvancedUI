// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/ListEntryBase.h"
#include "ListEntryString.generated.h"

class UFrontendCommonButtonBase;
class UAdvancedCommonRotator;
class UListDataObject_String;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UListEntryString : public UListEntryBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	virtual void OnToggleEditableState(bool bIsEditable) override;

private:
	void OnPreviousOptionButtonClicked();
	void OnNextOptionButtonClicked();
	void OnRotatorValueChanged(int32 Value, bool bUserInitiated);

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget,AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* PreviousOptionCommonButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UAdvancedCommonRotator* OptionCommonRotator;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* NextOptionCommonButton;

	UPROPERTY(Transient)
	UListDataObject_String* CachedStringDataObject;

};
