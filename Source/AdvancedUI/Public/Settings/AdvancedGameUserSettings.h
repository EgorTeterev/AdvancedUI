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
	static UAdvancedGameUserSettings* Get();
	
private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
};
