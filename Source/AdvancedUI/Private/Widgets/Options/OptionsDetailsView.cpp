// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDetailsView.h"
#include "Widgets/Options/DataObjects/ListDataObjectBase.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"

void UOptionsDetailsView::UpdateDetailsViewInfo(UListDataObjectBase* DataObject, const FString& EntryWidgetClassName)
{
	if (!DataObject)
	{
		return;
	}

	Title->SetText(DataObject->GetDataDisplayName());

	if (!DataObject->GetSoftDescriprionImage().IsNull())
	{
		DescriptionImage->SetBrushFromLazyTexture(DataObject->GetSoftDescriprionImage());
		DescriptionImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	Description->SetText(DataObject->GetDescriptionRichText());

	const FString DynamicDetailsText = FString::Printf(
		TEXT("Data object class: <Bold>%s</>\n\nEntry Widget Class: <Bold>%s</>"),
		*DataObject->GetClass()->GetName(),
		*EntryWidgetClassName
	);
	
	DynamicDetails->SetText(FText::FromString(DynamicDetailsText));
	DisableReason->SetText(DataObject->GetDisabledRichText());
}

void UOptionsDetailsView::ClearDetailsViewInfo()
{
	Title->SetText(FText::GetEmpty());
	DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	Description->SetText(FText::GetEmpty());
	DisableReason->SetText(FText::GetEmpty());
}

void UOptionsDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClearDetailsViewInfo();

}