// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendFunctionLibrary.h"
#include "Settings/UIDeveloperSettings.h"


TSoftClassPtr<UAdvancedActivatableWidget> UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag( UPARAM(meta = (Categories = "UI.Widget")) FGameplayTag WidgetTag)
{
    const UUIDeveloperSettings* DeveloperSettings = GetDefault<UUIDeveloperSettings>();

    checkf(DeveloperSettings->SoftWidgetsMap.Contains(WidgetTag),TEXT("Could not find widget by tag:%s in developer settings map"),*WidgetTag.ToString());

    return DeveloperSettings->SoftWidgetsMap.FindRef(WidgetTag);
}
