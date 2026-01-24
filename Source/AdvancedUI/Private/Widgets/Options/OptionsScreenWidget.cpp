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
	AUIDebug::ConsoleMessage(TEXT("Reset"));
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