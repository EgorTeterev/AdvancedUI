// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"
#include "ListDataObject_String.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UListDataObject_String : public UListDataObject_Value
{
	GENERATED_BODY()
	
public:
	void AddDynamicOption(const FString& AddOptionString, const FText& AddDisplayText);
	void SwitchToNextOption();
	void SwitchToPreviousOption();
	void OnRotatorInitiatedChange(const FText& NewSelectedText);

	FORCEINLINE const TArray<FText>& GetOptionsTextArray() const { return AvailableOptionsTextArray; };
	FORCEINLINE const FText GetCurrentDisplayText() const { return CurrentDisplayText; };



protected:
	virtual void OnDataObjectInitialized() override;
	bool TrySetDisplayTextFromStringValue(const FString& String);

	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

	virtual bool CanSetToForcedStringValue(const FString& ForcedValue) const override;
	virtual void OnSetToForcedStringValue(const FString& ForcedValue) override;

	TArray<FString> AvailableOptionsStringArray;
	TArray<FText> AvailableOptionsTextArray;
	FString CurrentStringValue;
	FText CurrentDisplayText;

};

//------------------------------------------------------------------------------------------------------------------------------------------------

UCLASS()
class ADVANCEDUI_API UListDataObject_StringBool : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void OverrideFalseDisplayText(const FText& NewFlaseDisplayText);
	void OverrideTrueDisplayText(const FText& NewTrueDisplayText);
	void SetTrueAsDefault();
	void SetFalseAsDefault();
	void TryInitBoolValue();

protected:
	virtual void OnDataObjectInitialized() override;

private:
	const FString TrueString = TEXT("true");
	const FString FalseString = TEXT("false");
};

//------------------------------------------------------------------------------------------------------------------------------------------------

UCLASS()
class ADVANCEDUI_API UListDataObject_StringEnum : public UListDataObject_String
{
	GENERATED_BODY()

public:
	template<typename EnumType>
	void AddEnumOption(EnumType EnumOption, const FText& DisplayText)
	{
		UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedToStringEnum = StaticEnumOption->GetNameStringByValue(EnumOption);

		AddDynamicOption(ConvertedToStringEnum, DisplayText);
	}

	template<typename EnumType>
	EnumType GetCurrentValueAsEnum()
	{
		UEnum* StaticEnumOption = StaticEnum<EnumType>();
		return StaticCast<EnumType>(StaticEnumOption->GetValueByNameString(CurrentStringValue));
	}

	template<typename EnumType>
	void SetDefaultEnumValue(EnumType DefaultValue)
	{
		UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedToStringEnum = StaticEnumOption->GetNameStringByValue(DefaultValue);
		SetDefaultValueFromString(ConvertedToStringEnum);
	}

};

//------------------------------------------------------------------------------------------------------------------------------------------------

UCLASS()
class ADVANCEDUI_API UListDataObject_StringInteger : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	void AddIntegerOption(int32 Value, const FText& DisplayText);

protected:
	virtual void OnDataObjectInitialized() override;
	virtual void OnDependencyObjectWasModified(UListDataObjectBase* ModifiedDependencyObject, EOptionsListDataModifyReason ModifyReason) override;
private:
	const FString TrueString = TEXT("true");
	const FString FalseString = TEXT("false");
};