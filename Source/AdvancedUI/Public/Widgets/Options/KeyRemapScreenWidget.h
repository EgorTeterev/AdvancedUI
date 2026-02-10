// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/AdvancedActivatableWidget.h"
#include "CommonInputTypeEnum.h"
#include "KeyRemapScreenWidget.generated.h"

class UCommonRichTextBlock;
class FkeyRemapInputPreprocessor;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UKeyRemapScreenWidget : public UAdvancedActivatableWidget
{
	GENERATED_BODY()

public:
	void SetDesiredInputType(ECommonInputType DesiredInputType) {CachedDesiredInputType = DesiredInputType;}

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

private:
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextMessage;

	TSharedPtr<FkeyRemapInputPreprocessor> CachedPreprocessor;

	ECommonInputType CachedDesiredInputType;
};
