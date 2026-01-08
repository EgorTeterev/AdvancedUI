// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/AdvancedActivatableWidget.h"
#include "OptionsScreenWidget.generated.h"

class UAdvancedCommonListView;
class UOptionsDataRegistry;
class UAdvancedTabListWidgetBase;

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UOptionsScreenWidget : public UAdvancedActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;

private:

	UOptionsDataRegistry* GetOrCreateDataRegistry();
	void OnResetBoundActionTriggered();
	void OnBackActionTriggered();

	//Bound Widgets
	UPROPERTY(meta=(BindWidget))
	UAdvancedTabListWidgetBase* OptionsTabListWidget;

	UPROPERTY(meta = (BindWidget))
	UAdvancedCommonListView* OptionsCommonListView;

	//Through this pointer is handled the creation of data in option screen
	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistery;


	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;

	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);

};
