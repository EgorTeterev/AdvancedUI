// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AdvancedActivatableWidget.h"
#include "Controllers/CustomPlayerController.h"

ACustomPlayerController* UAdvancedActivatableWidget::GetOwningPC()
{

	if (!CachedOwningPC.IsValid())
	{
		CachedOwningPC = GetOwningPlayer<ACustomPlayerController>();
	}

	return CachedOwningPC.IsValid() ? CachedOwningPC.Get() : nullptr;
}
