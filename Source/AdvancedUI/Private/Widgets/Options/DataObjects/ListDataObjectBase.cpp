// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObjectBase.h"
#include "Settings/AdvancedGameUserSettings.h"

void UListDataObjectBase::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObjectBase::AddEditCondition(const FOptionDataEditConditionDescriptor& EditCondition)
{
	EditConditionArray.Add(EditCondition);
}

bool UListDataObjectBase::IsDataCurrenlyEditable()
{
	bool bIsEditable = true;

	if (EditConditionArray.IsEmpty())
	{
		return bIsEditable;
	}
	
	FString DisableRichReason;
	for (const FOptionDataEditConditionDescriptor& Condition : EditConditionArray)
	{
		if (Condition.IsEditConditionMet() || !Condition.IsValid())
		{
			continue;
		}

		bIsEditable = false;

		DisableRichReason.Append(Condition.GetDisableRichReason());
		SetDisabledRichText(FText::FromString(DisableRichReason));

		if (Condition.HasForcedStringValue())
		{
			const FString ForcedStringValue = Condition.GetDisabledForcedStringValue();

			if (CanSetToForcedStringValue(ForcedStringValue))
			{
				OnSetToForcedStringValue(ForcedStringValue);
			}
		}
	}

	return bIsEditable;
}

void UListDataObjectBase::AddEditDependencyObject(UListDataObjectBase* ObjectToDependOn)
{
	if (!ObjectToDependOn->OnListDataModified.IsBoundToObject(this))
	{
		ObjectToDependOn->OnListDataModified.AddUObject(this, &ThisClass::OnDependencyObjectWasModified);
	}
}

void UListDataObjectBase::OnDataObjectInitialized()
{

}

void UListDataObjectBase::NotifyListDataModified(UListDataObjectBase* ModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);

	if (bShouldApplyChangeSettingsImmediatly)
	{
		UAdvancedGameUserSettings::Get()->ApplySettings(true);
	}
}

void UListDataObjectBase::OnDependencyObjectWasModified(UListDataObjectBase* ModifiedDependencyObject, EOptionsListDataModifyReason ModifyReason)
{
	OnDependecyObjectModified.Broadcast(ModifiedDependencyObject, ModifyReason);
}
