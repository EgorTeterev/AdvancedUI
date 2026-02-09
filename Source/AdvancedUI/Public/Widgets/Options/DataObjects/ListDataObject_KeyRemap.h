// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_Value.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObject_KeyRemap.generated.h"

class UEnhancedPlayerMappableKeyProfile;
class UEnhancedInputUserSettings;
/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UListDataObject_KeyRemap : public UListDataObject_Value
{
	GENERATED_BODY()

public:
	void InitKeyRemapData(
		UEnhancedInputUserSettings* OwningUserSettings,
		UEnhancedPlayerMappableKeyProfile* KeyProfile,
		ECommonInputType DesiredInputKeyType, const FPlayerKeyMapping& OwningPlayerKeyMapping
	);



private:
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedOwningInputUserSettings;

	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningPlayerMappableKeyProfile;

	ECommonInputType CachedDesiredInputKeyType;

	FName OwningMappingName;

	EPlayerMappableKeySlot CachedOwningKeySlot;
};
