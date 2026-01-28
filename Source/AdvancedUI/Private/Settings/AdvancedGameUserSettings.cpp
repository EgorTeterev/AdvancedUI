// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/AdvancedGameUserSettings.h"

UAdvancedGameUserSettings::UAdvancedGameUserSettings()
	:OverallVolume (1.0f)
{
}

UAdvancedGameUserSettings* UAdvancedGameUserSettings::UAdvancedGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UAdvancedGameUserSettings> (GEngine->GetGameUserSettings());
	}
	
	return nullptr;
}

void UAdvancedGameUserSettings::SetOverallVolume(float NewOverallVolume)
{
	OverallVolume = NewOverallVolume;
	
	//Handle volume changing
}

void UAdvancedGameUserSettings::SetMusicVolume(float NewMusicVolume)
{
	MusicVolume = NewMusicVolume;

	//Handle music volume changing
}
