// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"

void UListDataObject_KeyRemap::InitKeyRemapData(UEnhancedInputUserSettings* OwningUserSettings, UEnhancedPlayerMappableKeyProfile* KeyProfile, ECommonInputType DesiredInputKeyType, const FPlayerKeyMapping& OwningPlayerKeyMapping)
{
	CachedOwningInputUserSettings = OwningUserSettings;
	CachedOwningPlayerMappableKeyProfile = KeyProfile;

	CachedDesiredInputKeyType = DesiredInputKeyType;

	OwningMappingName = OwningPlayerKeyMapping.GetMappingName();

	CachedOwningKeySlot = OwningPlayerKeyMapping.GetSlot();
}
