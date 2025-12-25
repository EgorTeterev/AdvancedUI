// Fill out your copyright notice in the Description page of Project Settings.


#include "Async/AsyncPushConfirmScreen.h"
#include "Subsystems/UISubsystem.h"

UAsyncPushConfirmScreen* UAsyncPushConfirmScreen::PushConfirmScreen(const UObject* WorldContext, EConfirmScreenType ScreenType, FText ScreenTitleToShow, FText ScreenMessageToShow)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext,EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncPushConfirmScreen* NewConfirmScreen = NewObject<UAsyncPushConfirmScreen>();
			NewConfirmScreen->CachedOwningWorld = World;
			NewConfirmScreen->CachedScreenType = ScreenType;
			NewConfirmScreen->CachedTitle = ScreenTitleToShow;
			NewConfirmScreen->CachedMessage = ScreenMessageToShow;

			NewConfirmScreen->RegisterWithGameInstance(World);
			return NewConfirmScreen;
		}
	}
	return nullptr;
}

void UAsyncPushConfirmScreen::Activate()
{
	UUISubsystem::Get(CachedOwningWorld.Get())->PushConfirmScreenToModalStackAsync(
	CachedScreenType,
	CachedTitle,
	CachedMessage,
		[this](EConfirmScreenButtonType ClickedButtonType) {
			OnButtonClicked.Broadcast(ClickedButtonType);
			SetReadyToDestroy();
		}
	); 
}
