// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "CommonInputSubsystem.h"
#include "CommonInputBaseTypes.h"

void UListDataObject_KeyRemap::InitKeyRemapData(UEnhancedInputUserSettings* OwningUserSettings, UEnhancedPlayerMappableKeyProfile* KeyProfile, ECommonInputType DesiredInputKeyType, const FPlayerKeyMapping& OwningPlayerKeyMapping)
{
	CachedOwningInputUserSettings = OwningUserSettings;
	CachedOwningPlayerMappableKeyProfile = KeyProfile;

	CachedDesiredInputKeyType = DesiredInputKeyType;

	CachedOwningMappingName = OwningPlayerKeyMapping.GetMappingName();

	CachedOwningKeySlot = OwningPlayerKeyMapping.GetSlot();
}

FSlateBrush UListDataObject_KeyRemap::GetIconFromCurrentKey() const
{
	check(CachedOwningInputUserSettings);

	FSlateBrush Brush;

	UCommonInputSubsystem* CommonInputSystem = UCommonInputSubsystem::Get(CachedOwningInputUserSettings->GetLocalPlayer());
	check(CommonInputSystem);

	UCommonInputPlatformSettings* PlatformSettings = UCommonInputPlatformSettings::Get();


	if (PlatformSettings)
	{
		const bool bHasFoundBrush = PlatformSettings->TryGetInputBrush(
			Brush, 
			GetOwningKeyMapping()->GetCurrentKey(), 
			CachedDesiredInputKeyType,
			CommonInputSystem->GetCurrentGamepadName()
		);
	}

	return Brush;
}

void UListDataObject_KeyRemap::BindNewInputKey(const FKey& NewKey)
{
	check(CachedOwningInputUserSettings);

	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedOwningMappingName;
	KeyArgs.Slot = CachedOwningKeySlot;
	KeyArgs.NewKey = NewKey;

	FGameplayTagContainer TagContainer;

	CachedOwningInputUserSettings->MapPlayerKey(KeyArgs, TagContainer);
	CachedOwningInputUserSettings->SaveSettings();

	NotifyListDataModified(this);
}

FPlayerKeyMapping* UListDataObject_KeyRemap::GetOwningKeyMapping() const
{
	check(CachedOwningPlayerMappableKeyProfile);

	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = CachedOwningMappingName;
	KeyArgs.Slot = CachedOwningKeySlot;
	
	return CachedOwningPlayerMappableKeyProfile->FindKeyMapping(KeyArgs);
}
