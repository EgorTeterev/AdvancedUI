// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/ListEntryBase.h"
#include "ListEntryKeyRemap.generated.h"

class UFrontendCommonButtonBase;
class UListDataObject_KeyRemap;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UListEntryKeyRemap : public UListEntryBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject);
	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

private:
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget,AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButtonKeyRemap;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButtonKeyReset;

	UListDataObject_KeyRemap* CachedRemapObject;

	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();
	void OnKeyToRemapPressed(const FKey& PressedKey);
	void OnKeyRemapCanceled(const FString& CanceledReason);
};
