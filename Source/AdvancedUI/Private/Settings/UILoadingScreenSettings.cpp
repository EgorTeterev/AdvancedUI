// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/UILoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UUILoadingScreenSettings::GetLoadingWidgetScreenChecked() const
{
	checkf(!SoftLoadingScreenWidgetClass.IsNull(),TEXT("Assign Soft loading screen widget class in Loading Screen Settings "));

	TSubclassOf<UUserWidget> LoadedScreenWidget =SoftLoadingScreenWidgetClass.LoadSynchronous();

	return LoadedScreenWidget;
}
