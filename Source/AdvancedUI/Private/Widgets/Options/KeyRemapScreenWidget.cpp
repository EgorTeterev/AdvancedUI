// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/KeyRemapScreenWidget.h"
#include "CommonRichTextBlock.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/IInputProcessor.h"

class FkeyRemapInputPreprocessor : public IInputProcessor
{

public:
	FkeyRemapInputPreprocessor(ECommonInputType InputTypeToListen)
	:CachedInputTypeToListenTo(InputTypeToListen)
	{

	}

protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
	{

	}

	/** Mouse movement input */
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		return false;
	}

	/** Mouse button press */
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		return false;
	}

private:
	ECommonInputType CachedInputTypeToListenTo;
};

void UKeyRemapScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedPreprocessor = MakeShared<FkeyRemapInputPreprocessor>(CachedDesiredInputType);

	FSlateApplication::Get().RegisterInputPreProcessor(CachedPreprocessor);
}

void UKeyRemapScreenWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedPreprocessor);
	}
}
