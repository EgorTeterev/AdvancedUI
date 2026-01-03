// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsScreenWidget.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "DebugHelper.h"

void UOptionsScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ResetAction.IsNull())
	{
		return;
	}
	
	
	ResetActionHandle = RegisterUIActionBinding(FBindUIActionArgs(ResetAction, true, FSimpleDelegate::CreateUObject(this, &UOptionsScreenWidget::OnResetBoundActionTriggered)));

	RegisterUIActionBinding(
		FBindUIActionArgs(ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(
				this,
				&UOptionsScreenWidget::OnBackActionTriggered)
		)
	);
}

void UOptionsScreenWidget::OnResetBoundActionTriggered()
{
	DeactivateWidget();
}

void UOptionsScreenWidget::OnBackActionTriggered()
{
	AUIDebug::ConsoleMessage(TEXT("Back action works"));
	//DeactivateWidget();

}
