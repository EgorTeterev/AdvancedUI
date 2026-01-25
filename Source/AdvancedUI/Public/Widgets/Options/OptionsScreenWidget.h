// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/AdvancedActivatableWidget.h"
#include "Types/AdvancedUIEnumTypes.h"
#include "OptionsScreenWidget.generated.h"

class UOptionsDetailsView;
class UAdvancedCommonListView;
class UOptionsDataRegistry;
class UAdvancedTabListWidgetBase;
class UListDataObjectBase;

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UOptionsScreenWidget : public UAdvancedActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnDeactivated() override;
private:

	UOptionsDataRegistry* GetOrCreateDataRegistry();

	void OnResetBoundActionTriggered();
	void OnBackActionTriggered();
	void OnListViewItemSelected(UObject* SelectedItem);
	void OnListViewItemHovered(UObject* HoveredListItem, bool bWasHovered);


	FString TryGetEntryWidgetClassName(UObject* OwningListItem) const;

	void OnListViewListDataModified(UListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason);

	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);

	//Bound Widgets
	UPROPERTY(meta=(BindWidget))
	UAdvancedTabListWidgetBase* OptionsTabListWidget;

	UPROPERTY(meta = (BindWidget))
	UAdvancedCommonListView* OptionsCommonListView;

	UPROPERTY(meta = (BindWidget))
	UOptionsDetailsView* OptionsDetailsListEntryInfo;

	//Through this pointer is handled the creation of data in option screen
	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistery;

	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	UPROPERTY(Transient)
	TArray<UListDataObjectBase*> ResettableSettingsArray;
	bool bIsResettingData = false;

	FUIActionBindingHandle ResetActionHandle;

	
	
};
