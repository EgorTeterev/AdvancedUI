// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Subsystems/UISubsystem.h"

void UFrontendCommonButtonBase::SetButtonText(FText Text)
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

FText UFrontendCommonButtonBase::GetButtonDisplayText() const
{
	if (CommonTextBlock_Button)
	{
		return CommonTextBlock_Button->GetText();
	}
	return FText();
}

void UFrontendCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}

void UFrontendCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (CommonTextBlock_Button && GetCurrentTextStyleClass())
	{
		CommonTextBlock_Button->SetStyle(GetCurrentTextStyleClass());
	}
}


void UFrontendCommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (!ButtonDescriptionText.IsEmpty())
	{
		UUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDescriptionText);
	}
}

void UFrontendCommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (!ButtonDescriptionText.IsEmpty())
	{
		UUISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, FText ());
	}
}