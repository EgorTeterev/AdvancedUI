// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "Widgets/AdvancedActivatableWidget.h"
#include "AsyncPushSoftWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushedSoftWidgetDelegate, UAdvancedActivatableWidget*, PushedWidget);

UCLASS()
class ADVANCEDUI_API UAsyncPushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable,meta=(WorldContext = "WorldContext",HidePin = "WorldContext",BlueprintInternalUseOnly = "true",DisplayName="PushSoftWidgetToWidgetStack"))
	static UAsyncPushSoftWidget* PushSoftWidget(const UObject* WorldContext, APlayerController* OwningPC,
		TSoftClassPtr<UAdvancedActivatableWidget>WidgetClass,
		UPARAM(meta = (Categories = "UI.WidgetStack")) FGameplayTag WidgetStackTag, bool FocusPushedWidget = true);
	
	UPROPERTY(BlueprintAssignable)
	FOnPushedSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushedSoftWidgetDelegate OnWidgetAfterPush;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	TSoftClassPtr<UAdvancedActivatableWidget> CachedSoftWidgetClass;
	FGameplayTag CachedStackTag;

	bool bCachedFocusOnWidget = false;


};
