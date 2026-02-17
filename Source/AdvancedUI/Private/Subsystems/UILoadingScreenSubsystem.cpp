// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UILoadingScreenSubsystem.h"
#include "Settings/UILoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/UILoadingScreenInterface.h"
#include "PreLoadScreenManager.h"

#include "DebugHelper.h"

void UUILoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();
}

bool UUILoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		AUIDebug::ConsoleMessage(TEXT("Subsystem should be created"));
		return FoundClasses.IsEmpty();
	}

	return false;
}

/** Implement this for initialization of instances of the system */
void UUILoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreloaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
	AUIDebug::ConsoleMessage(TEXT("Subsystem initialized"));
}

/** Implement this for deinitialization of instances of the system */
void UUILoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

void UUILoadingScreenSubsystem::OnMapPreloaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance())
	{
		AUIDebug::ConsoleMessage(TEXT("Something wrond with game instance,map cant load"));

		return;
	}

	SetTickableTickType(ETickableTickType::Conditional);
	AUIDebug::ConsoleMessage(TEXT("Map is loading"));

	bIsCurrentlyLoadingMap = true;

	TryUpdateLoadingScreen();
}

void UUILoadingScreenSubsystem::OnMapPostLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() != GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}
}

void UUILoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	AUIDebug::ConsoleMessage(TEXT("Trying update loading screen"));

	if (IsLoadingScreenActive())
	{
		AUIDebug::ConsoleMessage(TEXT("Loading screen is already active"));

		return;
	}

	if (ShouldShowLoadingScreen())
	{
		AUIDebug::ConsoleMessage(TEXT("Should show loading screen"));
		TryDisplayLoadingScreen();

		OnLoadingReasonChanged.Broadcast(CurrentLoadingReason);
	}
	else
	{
		AUIDebug::ConsoleMessage(TEXT("Should not show loading screen"));

		TryRemoveLoadingScreen();

		HoldLoadingScreenStartUpTime = -1.f;

		SetTickableTickType(ETickableTickType::Never);
	}
}

void UUILoadingScreenSubsystem::TryDisplayLoadingScreen()
{
	if (CachedCreatedLoadingScreen)
	{
		AUIDebug::ConsoleMessage(TEXT("Loading screen is already exists"));
		return;
	}

	const UUILoadingScreenSettings* LoadingScreenSetting = GetDefault<UUILoadingScreenSettings>();

	TSubclassOf<UUserWidget> LoadedWidgetClass = LoadingScreenSetting->GetLoadingWidgetScreenChecked();

	UUserWidget* CreatedWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadedWidgetClass, NAME_None);

	check(CreatedWidget);
	CachedCreatedLoadingScreen = CreatedWidget->TakeWidget();

	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(
		CachedCreatedLoadingScreen.ToSharedRef(),
		10
	);

	NotifyLoadingScreenVisibilityChanged(true);
}

void UUILoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (!CachedCreatedLoadingScreen)
	{
		AUIDebug::ConsoleMessage(TEXT("Nothing to remove"));

		return;
	}

	AUIDebug::ConsoleMessage(TEXT("Trying to remove loading screen"));

	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedLoadingScreen.ToSharedRef());
	CachedCreatedLoadingScreen.Reset();

	NotifyLoadingScreenVisibilityChanged(false);
}

void UUILoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(bool bIsVisible)
{
	for (const ULocalPlayer* ExistingLP : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistingLP)
		{
			continue;
		}

		APlayerController* PC = ExistingLP->GetPlayerController(GetGameInstance()->GetWorld());

		if (PC)
		{
			if (PC->Implements<UUILoadingScreenInterface>())
			{
				if (bIsVisible)
				{
					IUILoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
				}
				else
				{
					IUILoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);
				}
			}

			APawn* OwningPawn = PC->GetPawn();

			if (OwningPawn)
			{
				if (OwningPawn->Implements<UUILoadingScreenInterface>())
				{
					if (bIsVisible)
					{
						IUILoadingScreenInterface::Execute_OnLoadingScreenActivated(OwningPawn);
					}
					else
					{
						IUILoadingScreenInterface::Execute_OnLoadingScreenDeactivated(OwningPawn);
					}
				}
			}
		}
	}
}

bool UUILoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UUILoadingScreenSettings* LoadingScreenSettings = GetDefault<UUILoadingScreenSettings>();

	if (GIsEditor && !LoadingScreenSettings->bSouldShowLoadingScreenInEditor)
	{
		return false;
	}

	if (CheckTheNeedToShowLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;

		return true;
	}

	CurrentLoadingReason = TEXT("Waiting for texture streaming");

	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

	const float CurrentTime = FPlatformTime::Seconds();

	if (HoldLoadingScreenStartUpTime < 0.f)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}

	const float ElapsedTime = CurrentTime - HoldLoadingScreenStartUpTime;

	if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenSomeExtraSeconds)
	{
		return true;
	}

	return false;
}

bool UUILoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	if (bIsCurrentlyLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading level");

		return true;
	}

	UWorld* OwningWorld = GetGameInstance()->GetWorld();

	if (!OwningWorld)
	{
		CurrentLoadingReason = TEXT("Initializing World");

		return true;
	}

	if (!OwningWorld->GetFirstPlayerController())
	{
		CurrentLoadingReason = TEXT("Player controller is not created yet");
	}

	return false;
}

bool UUILoadingScreenSubsystem::IsLoadingScreenActive() const
{
	FPreLoadScreenManager* PreLoadManager= FPreLoadScreenManager::Get();

	if (PreLoadManager)
	{
		return PreLoadManager->HasValidActivePreLoadScreen();
	}
	
	return false;
}

UWorld* UUILoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGI = GetGameInstance())
	{
		return OwningGI->GetWorld();
	}

	return nullptr;
}

bool UUILoadingScreenSubsystem::IsTickable() const
{
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UUILoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UUILoadingScreenSubsystem,STATGROUP_Tickables)
}

ETickableTickType UUILoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}

	return ETickableTickType::Conditional;
}