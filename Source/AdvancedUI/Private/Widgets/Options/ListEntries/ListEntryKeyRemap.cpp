// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/ListEntryKeyRemap.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Subsystems/UISubsystem.h"
#include "Tags/UIGameplayTags.h"
#include "FrontendFunctionLibrary.h"
#include "Widgets/Options/KeyRemapScreenWidget.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

#include "DebugHelper.h"


void UListEntryKeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CommonButtonKeyRemap->OnClicked().AddUObject(this,&ThisClass::OnRemapKeyButtonClicked);
	CommonButtonKeyReset->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void UListEntryKeyRemap::OnOwningListDataObjectSet(UListDataObjectBase* OwningListDataObject)
{
	Super::OnOwningListDataObjectSet(OwningListDataObject);
	CachedRemapObject = CastChecked<UListDataObject_KeyRemap>(OwningListDataObject);

	CommonButtonKeyRemap->SetButtonDisplayImage(CachedRemapObject->GetIconFromCurrentKey());
}

void UListEntryKeyRemap::OnOwningListDataObjectModified(UListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedRemapObject)
	{
		CommonButtonKeyRemap->SetButtonDisplayImage(CachedRemapObject->GetIconFromCurrentKey());
	}
}

void UListEntryKeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();

	UUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		UIGameplayTags::UI_WidgetStack_Modal,
		UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(UIGameplayTags::UI_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState, UAdvancedActivatableWidget* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UKeyRemapScreenWidget* CreatedKeyRemapScreen = CastChecked<UKeyRemapScreenWidget>(PushedWidget);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectionCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);

				if (CachedRemapObject)
				{
					CreatedKeyRemapScreen->SetDesiredInputType(CachedRemapObject->GetDesiredInputKeyType());
				}

			}
		}
	);
}

void UListEntryKeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (!CachedRemapObject)
	{
		return;
	}

	CachedRemapObject->TryResetBackToDefaultValue();
}

void UListEntryKeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedRemapObject)
	{
		CachedRemapObject->BindNewInputKey(PressedKey);
	}
}

void UListEntryKeyRemap::OnKeyRemapCanceled(const FString& CanceledReason)
{
	UUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::Ok,
		FText::FromString(TEXT("Key remap")),
		FText::FromString(CanceledReason),
		[](EConfirmScreenButtonType ClickedButton)
		{

		}
	);
}
