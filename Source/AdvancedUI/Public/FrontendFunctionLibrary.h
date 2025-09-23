// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h" 
#include "GameplayTagContainer.h"
#include "FrontendFunctionLibrary.generated.h"

class UAdvancedActivatableWidget; 
UCLASS()
class ADVANCEDUI_API UFrontendFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,Category = "Frontend Function Library")
	static TSoftClassPtr<UAdvancedActivatableWidget> GetFrontendSoftWidgetClassByTag(UPARAM( meta = (Categories = "UI.Widget")) FGameplayTag WidgetTag);
};
