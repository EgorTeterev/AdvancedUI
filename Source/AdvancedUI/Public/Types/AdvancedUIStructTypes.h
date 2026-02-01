#pragma once

#include "AdvancedUIStructTypes.generated.h"

USTRUCT()
struct FOptionDataEditConditionDescriptor
{
	GENERATED_BODY()

public:
	void SetEditCondition(TFunction<bool()> EditCondition)
	{
		EditConditionFunc = EditCondition;
	}

	bool IsValid() const
	{
		return EditConditionFunc != nullptr;
	}

	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunc();
		}
		
		return false;
	}

	FString GetDisableRichReason() const { return DisableRichReason; }

	void SetDisableRichReason(const FString& RichText)
	{
		DisableRichReason = RichText;
	}

	bool HasForcedStringValue() const
	{
		return DisabledForcedStringValue.IsSet();
	}

	FString GetDisabledForcedStringValue() const
	{
		return DisabledForcedStringValue.GetValue();
	}

	void SetDisabledForcedStringValue(const FString& ForcedValue)
	{
		DisabledForcedStringValue = ForcedValue;
	}


private:
	TFunction<bool()> EditConditionFunc;
	FString DisableRichReason;
	TOptional<FString> DisabledForcedStringValue;
};