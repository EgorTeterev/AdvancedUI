// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/AdvancedCommonRotator.h"
#include "CommonTextBlock.h"

void UAdvancedCommonRotator::SetSelectedOptionByText(const FText& OptionText)
{
	const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[OptionText](const FText& TextItem)->bool
		{
			return TextItem.EqualTo(OptionText);
		}
	);

	if (FoundIndex != INDEX_NONE)
	{
		SetSelectedItem(FoundIndex);
	}
	else
	{
		MyText->SetText(OptionText);
	}
}