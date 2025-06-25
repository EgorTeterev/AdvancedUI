// Fill out your copyright notice in the Description page of Project Settings.


#include "Async/AsyncPushSoftWidget.h"

UAsyncPushSoftWidget* UAsyncPushSoftWidget::PushSoftWidget(const UObject* WorldContext, APlayerController* OwningPC, TSoftClassPtr<UAdvancedActivatableWidget> WidgetClass, UPARAM(meta = (Categories = "UI.WidgetStack"))FGameplayTag WidgetStackTag, bool FocusPushedWidget)
{
	checkf(!WidgetClass.IsNull(), TEXT("Pushed null soft widget class to widgetStack with tag:%s"), *WidgetStackTag.ToString());
	
	if(GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
		
		if (World)
		{
			UAsyncPushSoftWidget* Node = NewObject<UAsyncPushSoftWidget>();
			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = OwningPC;
			Node->CachedSoftWidgetClass = WidgetClass;
			Node->CachedStackTag = WidgetStackTag;
			Node->bCachedFocusOnWidget = FocusPushedWidget;

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}

	return nullptr;
}
