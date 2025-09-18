// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "UIDeveloperSettings.generated.h"
class UAdvancedActivatableWidget;
/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Frontend UI Settings"))
class ADVANCEDUI_API UUIDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config,EditAnywhere,Category = "Widget soft pointer", meta = (ForceInlineRow,Categories = "UI.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UAdvancedActivatableWidget>> SoftWidgetsMap;
	
};
