// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/KeyRemapScreenWidget.h"
#include "CommonRichTextBlock.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/IInputProcessor.h"
#include "CommonInputSubsystem.h"
#include "ICommonInputModule.h"
#include "CommonUITypes.h"
#include "DebugHelper.h"

class FkeyRemapInputPreprocessor : public IInputProcessor
{

public:
	FkeyRemapInputPreprocessor(ECommonInputType InputTypeToListen,ULocalPlayer* LocalPlayer)
	:CachedInputTypeToListenTo(InputTypeToListen)
	,CachedOwningPlayerWeak(LocalPlayer)
	{

	}

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedDelegate,const FKey&);
	FOnInputPreProcessorKeyPressedDelegate OnInputPreProcessorKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectedDelegate, const FString&);
	FOnInputPreProcessorKeySelectedDelegate OnInputPreProcessorKeySelectedCanceled;

protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
	{

	}

	/** Mouse movement input */
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());
		
		return false;
	}

	/** Mouse button press */
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());
		
		return false;
	}

	void ProcessPressedKey(const FKey& PressedKey)
	{
		if (PressedKey == EKeys::Escape)
		{
			OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("Key remap canceled."));
			
			return;
		}

		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedOwningPlayerWeak.Get());
		check(CommonInputSubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();


		switch (CachedInputTypeToListenTo)
		{
		case ECommonInputType::MouseAndKeyboard:
			
			if (PressedKey.IsGamepadKey() || CurrentInputType == ECommonInputType::Gamepad)
			{
				OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("Gamepad key pressed for keyboard option."));

				return;
			}
			break;
		case ECommonInputType::Gamepad:

			if (CurrentInputType == ECommonInputType::Gamepad && PressedKey == EKeys::LeftMouseButton)
			{
				FCommonInputActionDataBase* InputActionDataBase = ICommonInputModule::GetSettings().GetDefaultClickAction().GetRow<FCommonInputActionDataBase>(TEXT(""));

				check(InputActionDataBase);

				OnInputPreProcessorKeyPressed.ExecuteIfBound(InputActionDataBase->GetDefaultGamepadInputTypeInfo().GetKey());
			}
			if (!PressedKey.IsGamepadKey())
			{
				OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("Keyboard key pressed for gamepad option."));

				return;
			}
			break;
		default:
			break;
		}

		OnInputPreProcessorKeyPressed.ExecuteIfBound(PressedKey);
	}

private:
	ECommonInputType CachedInputTypeToListenTo;
	TWeakObjectPtr<ULocalPlayer> CachedOwningPlayerWeak;
};

void UKeyRemapScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedPreprocessor = MakeShared<FkeyRemapInputPreprocessor>(CachedDesiredInputType,GetOwningLocalPlayer());
	CachedPreprocessor->OnInputPreProcessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPressed);
	CachedPreprocessor->OnInputPreProcessorKeySelectedCanceled.BindUObject(this, &ThisClass::OnInvalidKeyPressed);

	FSlateApplication::Get().RegisterInputPreProcessor(CachedPreprocessor);

	FString DeviseName;

	switch (CachedDesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		DeviseName = TEXT("Mouse & Keyboard");
		break;
	case ECommonInputType::Gamepad:
		DeviseName = TEXT("Gamepad");
		break;
	default:
		break;
	}

}

void UKeyRemapScreenWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedPreprocessor);
	}
}

void UKeyRemapScreenWidget::OnValidKeyPressed(const FKey& PressedKey)
{
	RequestDeactivateWidget(
		[this, PressedKey]()
		{
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
		}
	);
}

void UKeyRemapScreenWidget::OnInvalidKeyPressed(const FString& CancelReason)
{
	RequestDeactivateWidget(
		[this, CancelReason]()
		{
			OnKeyRemapScreenKeySelectionCanceled.ExecuteIfBound(CancelReason);
		}
	);
}

void UKeyRemapScreenWidget::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda(
			[this, PreDeactivateCallback](float DeltaTime)->bool
			{
				PreDeactivateCallback();

				DeactivateWidget();

				return false;
			}
		)
	);
}
