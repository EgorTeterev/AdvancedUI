// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Types/AdvancedUIEnumTypes.h"
#include "ListEntryBase.generated.h"

class UCommonTextBlock;
class UListDataObjectBase;
/**
 *
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UListEntryBase : public UCommonUserWidget , public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	//The child widget class can override this function to handle initialization.
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject);

	//The child widget class override this function to update UI values after its data was modified.
	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

private:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidgetOptional,AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonTextDisplayName;
};
