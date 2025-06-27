// Fill out your copyright notice in the Description page of Project Settings.


#include "Async/AsyncPushSoftWidget.h"
#include "Subsystems/UISubsystem.h"

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

void UAsyncPushSoftWidget::Activate()
{
	UUISubsystem* UISubsystem = UUISubsystem::Get(CachedOwningWorld.Get());
	UISubsystem->PushSoftWidgetToStackAsync(CachedStackTag, CachedSoftWidgetClass,
		[this](EAsyncPushWidgetState PushState, UAdvancedActivatableWidget* PushedWidget)
		{
			switch (PushState)
			{
			case EAsyncPushWidgetState::AfterPush:

				PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
				OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
				break;
			case EAsyncPushWidgetState::OnCreatedBeforePush:

				OnWidgetAfterPush.Broadcast(PushedWidget);
				if (bCachedFocusOnWidget)
				{
					if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
					{
						WidgetToFocus->SetFocus();
					}
				}
				break;
			default:
				break;
			}
		}
	);
}