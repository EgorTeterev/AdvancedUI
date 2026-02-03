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

public:
	UFUNCTION(BlueprintImplementableEvent,meta=(DisplayName ="On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered,bool bIsEntryWidgetStillSelected);
	void NativeOnListEntryWidgetHovered(bool bWasHovered);

protected:
	//Child widget blueprint should override this function for the gamepad interaction
	UFUNCTION(BlueprintImplementableEvent,meta = (DisplayName = "Get Widget To Focus For Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//The child widget class can override this function to handle initialization.
	virtual void OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject);

	//The child widget class override this function to update UI values after its data was modified.
	virtual void OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

	virtual void OnOwningDependencyObjectModified(UListDataObjectBase* OwningDependencyData, EOptionsListDataModifyReason ModifyReason);

	//The child should override to change editable state of widgets and custom visual
	virtual void OnToggleEditableState(bool bIsEditable);

	void SelectThisEntryWidget();

private:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidgetOptional,AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonTextDisplayName;

	UPROPERTY(Transient)
	UListDataObjectBase* CachedOwnDataObject;
};
