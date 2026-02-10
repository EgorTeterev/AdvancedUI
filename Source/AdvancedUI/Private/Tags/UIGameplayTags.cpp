// Fill out your copyright notice in the Description page of Project Settings.


#include "Tags/UIGameplayTags.h"

namespace UIGameplayTags
{
	//Widget Stacks
	ADVANCEDUI_API UE_DEFINE_GAMEPLAY_TAG(UI_WidgetStack_Modal,"UI.WidgetStack.Modal");
	ADVANCEDUI_API UE_DEFINE_GAMEPLAY_TAG(UI_WidgetStack_GameMenu,"UI.WidgetStack.GameMenu");
	ADVANCEDUI_API UE_DEFINE_GAMEPLAY_TAG(UI_WidgetStack_GameHud,"UI.WidgetStack.GameHud");
	ADVANCEDUI_API UE_DEFINE_GAMEPLAY_TAG(UI_WidgetStack_Frontend,"UI.WidgetStack.Frontend");

	//Widget soft classes
	ADVANCEDUI_API UE_DEFINE_GAMEPLAY_TAG(UI_Widget_PressAnyKeyScreen, "UI.Widget.PressAnyKeyScreen");
	ADVANCEDUI_API UE_DEFINE_GAMEPLAY_TAG(UI_Widget_MainMenuScreen, "UI.Widget.MainMenuScreen");
	ADVANCEDUI_API UE_DEFINE_GAMEPLAY_TAG(UI_Widget_ConfirmScreen, "UI.Widget.ConfirmScreen");
	ADVANCEDUI_API UE_DEFINE_GAMEPLAY_TAG(UI_Widget_KeyRemapScreen, "UI.Widget.KeyRemapScreen");

	//Options soft images
	ADVANCEDUI_API UE_DEFINE_GAMEPLAY_TAG(UI_Image_TestImage, "UI.Image.TestImage");

}