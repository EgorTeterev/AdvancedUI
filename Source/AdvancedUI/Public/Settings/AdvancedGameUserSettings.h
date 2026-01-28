// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "AdvancedGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UAdvancedGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UAdvancedGameUserSettings();

	static UAdvancedGameUserSettings* Get();
	
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; };

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& NewDifficulty) { CurrentGameDifficulty = NewDifficulty; };

	UFUNCTION()
	float GetOverallVolume() const { return OverallVolume; };

	UFUNCTION()
	void SetOverallVolume(float NewOverallVolume);

	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; };

	UFUNCTION()
	void SetMusicVolume(float NewMusicVolume);

	UFUNCTION()
	float GetSoundFXVolume() const { return SoundFXVolume; };

	UFUNCTION()
	void SetSoundFXVolume(float NewSoundFXVolume);

private:
	//Game collection tab
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	
	//Audio collection tab
	UPROPERTY(Config)
	float OverallVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	float SoundFXVolume;

};
