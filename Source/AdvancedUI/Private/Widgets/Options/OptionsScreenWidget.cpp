// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsScreenWidget.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Components/AdvancedTabListWidgetBase.h"
#include "Widgets/Options/DataObjects/ListDataObjectCollection.h"
#include "Widgets/Components/AdvancedCommonListView.h"
#include "Settings/AdvancedGameUserSettings.h"
#include "Widgets/Options/ListEntries/ListEntryBase.h"
#include "Widgets/Options/OptionsDetailsView.h"
#include "Subsystems/UISubsystem.h"
#include "DebugHelper.h"

void UOptionsScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(FBindUIActionArgs(ResetAction, true, FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)));
	}
	
	RegisterUIActionBinding(
		FBindUIActionArgs(ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(
				this,
				&ThisClass::OnBackActionTriggered)
		)
	);

	OptionsTabListWidget->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);

	OptionsCommonListView->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
	OptionsCommonListView->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
}
void UOptionsScreenWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UAdvancedGameUserSettings::Get()->ApplySettings(true);
}

void UOptionsScreenWidget::OnResetBoundActionTriggered()
{
	if (ResettableSettingsArray.IsEmpty())
	{
		return;
	}

	UCommonButtonBase* SelectedButton = OptionsTabListWidget->GetTabButtonBaseByID(OptionsTabListWidget->GetActiveTab());

	const FString SelectedTabButtonName = CastChecked<UFrontendCommonButtonBase>(SelectedButton)->GetButtonDisplayText().ToString();

	//TFunction<void(EConfirmScreenButtonType)> ButtonCallback
	UUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesOrNo,
		FText::FromString(TEXT("Reset")),
		FText::FromString(TEXT("Are you sure you want to reset all the settings under the ")+SelectedTabButtonName +TEXT(" tab.")),
		[this](EConfirmScreenButtonType ClickedButtonType) 
		{
			if (ClickedButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;
			}

			bIsResettingData = true;
			bool bFailedToResetData = false;

			for (UListDataObjectBase* OptionToReset : ResettableSettingsArray)
			{
				if (!OptionToReset)
				{
					continue;
				}

				if (OptionToReset->TryResetBackToDefaultValue())
				{

				}
				else
				{
					bFailedToResetData = true;
				}
			}

			bIsResettingData = false;

			if (!bFailedToResetData)
			{
				ResettableSettingsArray.Empty();
				RemoveActionBinding(ResetActionHandle);
			}
		});
}

void UOptionsScreenWidget::OnBackActionTriggered()
{
	DeactivateWidget();
}

void UOptionsScreenWidget::OnListViewItemSelected(UObject* SelectedItem)
{
	if (!SelectedItem)
	{
		return;
	}

	OptionsDetailsListEntryInfo->UpdateDetailsViewInfo(CastChecked<UListDataObjectBase>(SelectedItem), TryGetEntryWidgetClassName(SelectedItem));

}

void UOptionsScreenWidget::OnListViewItemHovered(UObject* HoveredListItem, bool bWasHovered)
{
	if (!HoveredListItem)
	{
		return;
	}

	UListEntryBase* HoveredEnryWidget = OptionsCommonListView->GetEntryWidgetFromItem<UListEntryBase>(HoveredListItem);

	check(HoveredEnryWidget);

	HoveredEnryWidget->NativeOnListEntryWidgetHovered(bWasHovered);

	if (bWasHovered)
	{
		OptionsDetailsListEntryInfo->UpdateDetailsViewInfo(CastChecked<UListDataObjectBase>(HoveredListItem), TryGetEntryWidgetClassName(HoveredListItem));
	}
	else
	{
		if (UListDataObjectBase* SelectedItemData = OptionsCommonListView->GetSelectedItem<UListDataObjectBase>())
		{
			OptionsDetailsListEntryInfo->UpdateDetailsViewInfo(SelectedItemData,TryGetEntryWidgetClassName(SelectedItemData));
		}
	}
}

void UOptionsScreenWidget::OnOptionsTabSelected(FName TabID)
{
	OptionsDetailsListEntryInfo->ClearDetailsViewInfo();
	TArray<UListDataObjectBase*> FoundListOfTabItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabID);

	OptionsCommonListView->SetListItems(FoundListOfTabItems);
	OptionsCommonListView->RequestRefresh();

	if (OptionsCommonListView->GetNumItems() != 0)
	{
		OptionsCommonListView->NavigateToIndex(0);
		OptionsCommonListView->SetSelectedIndex(0);
	}

	ResettableSettingsArray.Empty();

	for (UListDataObjectBase* ListItem : FoundListOfTabItems)
	{
		if (!ListItem)
		{
			continue;
		}

		if (!ListItem->OnListDataModified.IsBoundToObject(this))
		{
			ListItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewListDataModified);
		}

		if (ListItem->CanResetBackToDefaultValue())
		{
			ResettableSettingsArray.AddUnique(ListItem);
		}
	}
	if (ResettableSettingsArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	} 
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

UOptionsDataRegistry* UOptionsScreenWidget::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistery)
	{
		CreatedOwningDataRegistery = NewObject<UOptionsDataRegistry>();
		CreatedOwningDataRegistery->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}

	checkf(CreatedOwningDataRegistery,TEXT("Data registry in not valid	"));

	return CreatedOwningDataRegistery;
}

void UOptionsScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (auto TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionCollections())
	{
		if (!TabCollection)
		{
			continue;
		}
		const FName TabID = TabCollection->GetDataID();

		if (OptionsTabListWidget->GetTabButtonBaseByID(TabID) != nullptr)
		{
			continue;
		}

		OptionsTabListWidget->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());

	}
}

FString UOptionsScreenWidget::TryGetEntryWidgetClassName(UObject* OwningListItem) const
{
	if (UUserWidget* FoundEntryWidget = OptionsCommonListView->GetEntryWidgetFromItem(OwningListItem))
	{
		return FoundEntryWidget->GetClass()->GetName();
	}

	return TEXT("Entry widget is not valid");
}

void UOptionsScreenWidget::OnListViewListDataModified(UListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{

	if (!ModifiedData || bIsResettingData)
	{
		return;
	}

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableSettingsArray.AddUnique(ModifiedData);

		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableSettingsArray.Contains(ModifiedData))
		{
			ResettableSettingsArray.Remove(ModifiedData);
		}
	}

	if (ResettableSettingsArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}

UWidget* UOptionsScreenWidget::NativeGetDesiredFocusTarget() const
{
	UObject* SelectedObject = OptionsCommonListView->GetSelectedItem();
	
	if (SelectedObject)
	{
		UUserWidget* SelectedEntryWidget = OptionsCommonListView->GetEntryWidgetFromItem(SelectedObject);
		
		if (SelectedEntryWidget)
		{
			return SelectedEntryWidget;
		}
	}

	return Super::NativeGetDesiredFocusTarget();
}