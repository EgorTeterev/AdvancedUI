// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UListDataObject_StringResolution : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void InitResolutionValues();

protected:
	virtual void OnDataObjectInitialized() override;

private:
	FString ResToValueString(const FIntPoint& Resolution) const;
	FText ResToDisplayText(const FIntPoint& Resolution) const;

	FString MaximumAllowedResolution;
};
