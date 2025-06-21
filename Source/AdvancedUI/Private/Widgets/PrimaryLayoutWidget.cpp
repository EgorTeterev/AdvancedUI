// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PrimaryLayoutWidget.h"
#include "DebugHelper.h"

UCommonActivatableWidgetContainerBase* UPrimaryLayoutWidget::GetWidgetStackWithTag(const FGameplayTag& Tag) const
{
	checkf(RegisteredWidgetStackMap.Contains(Tag), TEXT("No stack widget with: %s name"), *Tag.ToString());

	return RegisteredWidgetStackMap.FindRef(Tag);
}

void UPrimaryLayoutWidget::RegisterWidgetStack(UPARAM(meta = (Categories = "UI.WidgetStack")) FGameplayTag StackTag, UCommonActivatableWidgetContainerBase* WidgetStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(StackTag))
		{
			RegisteredWidgetStackMap.Add(StackTag,WidgetStack);
			AUIDebug::ConsoleMessage(TEXT("Added to stack widget with tag") + StackTag.ToString());
		}
	}
}
