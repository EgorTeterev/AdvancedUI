// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/AdvancedActivatableWidget.h"
#include "OptionsScreenWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UOptionsScreenWidget : public UAdvancedActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
private:	

	void OnResetBoundActionTriggered();
	void OnBackActionTriggered();

	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;
};
