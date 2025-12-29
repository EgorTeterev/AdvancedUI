// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ConfirmScreenWidget.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "ICommonInputModule.h"
#include "CommonTextBlock.h"

UConfirmInfoObject* UConfirmInfoObject::CreateOKScreen(const FText& ScreenTitle, const FText& ScreenMessage)
{
    UConfirmInfoObject* NewOKConfirmScreen = NewObject<UConfirmInfoObject>();
    NewOKConfirmScreen->ScreenTitle = ScreenTitle;
    NewOKConfirmScreen->ScreenMessage = ScreenMessage;

    FConfirmScreenButtonInfo OkButtonInfo;
    OkButtonInfo.ButtonType = EConfirmScreenButtonType::Closed;
    OkButtonInfo.ButtonText = FText::FromString(TEXT("Ok"));

    NewOKConfirmScreen->AvailableScreenButtons.Add(OkButtonInfo);
    
    return NewOKConfirmScreen;
}

UConfirmInfoObject* UConfirmInfoObject::CreateYesOrNoScreen(const FText& ScreenTitle, const FText& ScreenMessage)
{
    UConfirmInfoObject* NewYesOrNoConfirmScreen = NewObject<UConfirmInfoObject>();
    NewYesOrNoConfirmScreen->ScreenTitle = ScreenTitle;
    NewYesOrNoConfirmScreen->ScreenMessage = ScreenMessage;

    FConfirmScreenButtonInfo YesButtonInfo;
    YesButtonInfo.ButtonType = EConfirmScreenButtonType::Confirmed;
    YesButtonInfo.ButtonText = FText::FromString(TEXT("Yes"));

    FConfirmScreenButtonInfo NoButtonInfo;
    NoButtonInfo.ButtonType = EConfirmScreenButtonType::Cancelled;
    NoButtonInfo.ButtonText = FText::FromString(TEXT("No"));

    NewYesOrNoConfirmScreen->AvailableScreenButtons.Add(YesButtonInfo);
    NewYesOrNoConfirmScreen->AvailableScreenButtons.Add(NoButtonInfo);


    return NewYesOrNoConfirmScreen;
}

UConfirmInfoObject* UConfirmInfoObject::CreateOkOrCancelScreen(const FText& ScreenTitle, const FText& ScreenMessage)
{
    UConfirmInfoObject* NewOkOrCancelConfirmScreen = NewObject<UConfirmInfoObject>();
    NewOkOrCancelConfirmScreen->ScreenTitle = ScreenTitle;
    NewOkOrCancelConfirmScreen->ScreenMessage = ScreenMessage;

    FConfirmScreenButtonInfo OkButtonInfo;
    OkButtonInfo.ButtonType = EConfirmScreenButtonType::Confirmed;
    OkButtonInfo.ButtonText = FText::FromString(TEXT("Ok"));

    FConfirmScreenButtonInfo CancelButtonInfo;
    CancelButtonInfo.ButtonType = EConfirmScreenButtonType::Cancelled;
    CancelButtonInfo.ButtonText = FText::FromString(TEXT("Cancel"));


    return NewOkOrCancelConfirmScreen;
}

void UConfirmScreenWidget::InitComfirmScreen(UConfirmInfoObject* ScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
    check(ScreenInfoObject && ConfirmScreenTitle && ConfirmScreenMessage && DynamicEntryBoxButtons);


    ConfirmScreenTitle->SetText(ScreenInfoObject->ScreenTitle);
    ConfirmScreenMessage->SetText(ScreenInfoObject->ScreenMessage);

    //if Entrybox has old buttons,that were created previosly - we should clear it
    if (DynamicEntryBoxButtons->GetNumEntries() != 0)
    {
        //Clear old buttons
        DynamicEntryBoxButtons->Reset<UFrontendCommonButtonBase>(
            [](UFrontendCommonButtonBase& ExistingButton)
            {
                ExistingButton.OnClicked().Clear();
            }
        );
    }

    //if array is empty - confirmation screen will have no buttons
    check(!ScreenInfoObject->AvailableScreenButtons.IsEmpty());

    for (const FConfirmScreenButtonInfo& ButtonInfoToSet : ScreenInfoObject->AvailableScreenButtons)
    {
        FDataTableRowHandle InputActionRowHandle;

        switch (ButtonInfoToSet.ButtonType)
        {
        case EConfirmScreenButtonType::Cancelled:
            InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
            break;

        case EConfirmScreenButtonType::Closed:
            InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
            break;

        default:
            break;
        }


        UFrontendCommonButtonBase* AddedButton = DynamicEntryBoxButtons->CreateEntry<UFrontendCommonButtonBase>();
        AddedButton->SetButtonText(ButtonInfoToSet.ButtonText);
        AddedButton->SetTriggeringInputAction(InputActionRowHandle);
        AddedButton->OnClicked().AddLambda(
            [ClickedButtonCallback, ButtonInfoToSet,this]() {
                ClickedButtonCallback(ButtonInfoToSet.ButtonType);
                DeactivateWidget();
            }
        );
    }

    //Focus on the last widget (if entry box is not empty)
    if (DynamicEntryBoxButtons->GetNumEntries() != 0)
    {
        DynamicEntryBoxButtons->GetAllEntries().Last()->SetFocus();
    }
}
