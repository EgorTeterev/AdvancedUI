// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "PrimaryLayoutWidget.generated.h"

class UCommonActivatableWidgetContainerBase;



UCLASS(Abstract,BlueprintType,meta=(DisableNaiveTick))
class ADVANCEDUI_API UPrimaryLayoutWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UCommonActivatableWidgetContainerBase* GetWidgetStackWithTag(const FGameplayTag& Tag) const;
protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories = "UI.WidgetStack")) FGameplayTag StackTag, UCommonActivatableWidgetContainerBase* WidgetStack);

private:

	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;
};
