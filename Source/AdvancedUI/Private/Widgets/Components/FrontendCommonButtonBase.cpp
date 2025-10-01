// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonTextBlock.h"

void UFrontendCommonButtonBase::SetButtonText(FText& Text)
{
	if (CommonTextBlock_Button && !Text.IsEmpty())
	{
		if (bUserUpperCaseForButtonText)
		{
			CommonTextBlock_Button->SetText(Text.ToUpper());
		}
		else
		{
			CommonTextBlock_Button->SetText(Text);
		}
	}
}

void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}

void UFrontendCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (CommonTextBlock_Button)
	{
		CommonTextBlock_Button->SetStyle(GetCurrentTextStyleClass());
	}
}