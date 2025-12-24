// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace UIGameplayTags
{
	//Widget Stacks
	ADVANCEDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_WidgetStack_Modal);
	ADVANCEDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_WidgetStack_GameMenu);
	ADVANCEDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_WidgetStack_GameHud);
	ADVANCEDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_WidgetStack_Frontend);

	//Widget soft classes
	ADVANCEDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_PressAnyKeyScreen);
	ADVANCEDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_MainMenuScreen);
	ADVANCEDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(UI_Widget_ConfirmScreen);

}