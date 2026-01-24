// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsDetailsView.generated.h"

class UListDataObjectBase;
class UCommonTextBlock;
class UCommonLazyImage;
class UCommonRichTextBlock;


/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UOptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateDetailsViewInfo(UListDataObjectBase* DataObject, const FString& EntryWidgetClassName = FString());
	void ClearDetailsViewInfo();

protected:

	virtual void NativeOnInitialized() override;
private:

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* DescriptionImage;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* Description;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* DynamicDetails;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* DisableReason;
};
