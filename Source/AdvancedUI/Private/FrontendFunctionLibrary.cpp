// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendFunctionLibrary.h"
#include "Settings/UIDeveloperSettings.h"


TSoftClassPtr<UAdvancedActivatableWidget> UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag( UPARAM(meta = (Categories = "UI.Widget")) FGameplayTag WidgetTag)
{
    const UUIDeveloperSettings* DeveloperSettings = GetDefault<UUIDeveloperSettings>();

    checkf(DeveloperSettings->SoftWidgetsMap.Contains(WidgetTag),TEXT("Could not find widget by tag:%s in developer settings soft widgets map"),*WidgetTag.ToString());

    return DeveloperSettings->SoftWidgetsMap.FindRef(WidgetTag);
}

TSoftObjectPtr<UTexture2D> UFrontendFunctionLibrary::GetOptionsSoftImageByTag(UPARAM(meta = (Categories = "UI.Image")) FGameplayTag ImageTag)
{
    const UUIDeveloperSettings* DeveloperSettings = GetDefault<UUIDeveloperSettings>();
    checkf(DeveloperSettings->OptionsSoftImagesMap.Contains(ImageTag), TEXT("Could not find image by tag:%s in developer settings image map"), *ImageTag.ToString());

    return DeveloperSettings->OptionsSoftImagesMap.FindRef(ImageTag);
}
