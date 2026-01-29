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

	TArray<FString> AvailableOptionsStringArray;
	TArray<FText> AvailableOptionsTextArray;
	FString CurrentStringValue;
	FText CurrentDisplayText;

};

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
