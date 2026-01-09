// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/AdvancedCommonListView.h"
#include "Widgets/Options/DataAssets/DA_DataListEntryMapping.h"
#include "Widgets/Options/ListEntries/ListEntryBase.h"
#include "Widgets/Options/DataObjects/ListDataObjectBase.h"
#include "Editor/WidgetCompilerLog.h"


UUserWidget& UAdvancedCommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	if (TSubclassOf<UListEntryBase> FoundWidgetClass = DataListEntryMapping->FindEntryClassByDataObject(CastChecked<UListDataObjectBase>(Item)))
	{
		return GenerateTypedEntry<UListEntryBase>(FoundWidgetClass, OwnerTable);

	}
	else
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
}



#if WITH_EDITOR
void UAdvancedCommonListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!DataListEntryMapping)
	{
		CompileLog.Error(FText::FromString(TEXT("DataListEntryMapping has no valid data asset assigned") + GetClass()->GetName()+TEXT(" needs valid data asset")));
	}
}
#endif

