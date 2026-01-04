// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsScreenWidget.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Components/AdvancedTabListWidgetBase.h"
#include "Widgets/Options/DataObjects/ListDataObjectCollection.h"
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

}

void UOptionsScreenWidget::OnResetBoundActionTriggered()
{
	AUIDebug::ConsoleMessage(TEXT("Reset"));
}

void UOptionsScreenWidget::OnBackActionTriggered()
{
	DeactivateWidget();
}

void UOptionsScreenWidget::OnOptionsTabSelected(FName TabId)
{

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
		const FName TabId = TabCollection->GetDataId();

		if (OptionsTabListWidget->GetTabButtonBaseByID(TabId) != nullptr)
		{
			continue;
		}

		OptionsTabListWidget->RequestRegisterTab(TabId, TabCollection->GetDataDisplayName());

	}
}
