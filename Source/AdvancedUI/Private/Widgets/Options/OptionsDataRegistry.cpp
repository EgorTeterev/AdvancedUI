// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObjectCollection.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendFunctionLibrary.h"
#include "Tags/UIGameplayTags.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Settings/AdvancedGameUserSettings.h"


#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
    MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UAdvancedGameUserSettings, SetterOrGetterFuncName))


void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* OwningPlayer)
{
	InitGameplayTab();
	InitAudioTab();
	InitVideoTab();
	InitControlTab();
}

TArray<UListDataObjectBase*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& SelectedTabID) const
{
	UListDataObjectCollection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate([SelectedTabID](UListDataObjectBase* AvailableTabCollection) -> bool
		{
		return (AvailableTabCollection->GetDataID() == SelectedTabID);
		}
	);

	checkf(FoundTabCollectionPtr, TEXT("No valid tab found under the ID %s"), *SelectedTabID.ToString());

	UListDataObjectCollection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UListDataObjectBase*> AllChildListItems;

	for (UListDataObjectBase* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
		{
			continue;
		}
		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UOptionsDataRegistry::InitGameplayTab()
{
	UListDataObjectCollection* NewGameplayOptionsCollection = NewObject<UListDataObjectCollection>();
	NewGameplayOptionsCollection->SetDataID(FName("GameplayOptionsCollection"));
	NewGameplayOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));


	//Game Difficulty
	{
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();

		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));
		GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the difficulty of the game.\n\n<Bold>Easy:</> Focuses on the story experiance.\n\n<Bold>Normal:</>Offers slighty harder combat experiance.\n\n<Bold>Hard:</> Offers a challenging combat experiance.Try,if you brave enough")));
		GameDifficulty->AddDynamicOption(TEXT("Easy"),FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplyChangeSettingsImmediatly(true);

		NewGameplayOptionsCollection->AddChildList(GameDifficulty);
	}

	//Test long option
	{
		UListDataObject_String* TestImage = NewObject<UListDataObject_String>();

		TestImage->SetDataID(FName("Really long name test"));
		TestImage->SetDataDisplayName(FText::FromString(TEXT("Really long name of option in list to check widget")));
		TestImage->SetSoftDescriprionImage(UFrontendFunctionLibrary::GetOptionsSoftImageByTag(UIGameplayTags::UI_Image_TestImage));
		NewGameplayOptionsCollection->AddChildList(TestImage);
	}

	RegisteredOptionsTabCollections.Add(NewGameplayOptionsCollection);
}

void UOptionsDataRegistry::InitAudioTab()
{
	UListDataObjectCollection* NewAudioOptionsCollection = NewObject<UListDataObjectCollection>();
	NewAudioOptionsCollection->SetDataID(FName("AudioOptionsCollection"));
	NewAudioOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	//Volume Category
	{
		UListDataObjectCollection* VolumeCategoryCollection = NewObject<UListDataObjectCollection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategory"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));

		NewAudioOptionsCollection->AddChildList(VolumeCategoryCollection);

		//Overall Volume
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(FText::FromString(TEXT("Overall Volume")));
			OverallVolume->SetDescriptionRichText(FText::FromString(TEXT("All volume")));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f,1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f,2.f));
			OverallVolume->SetSliderStep(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplyChangeSettingsImmediatly(true);

			VolumeCategoryCollection->AddChildList(OverallVolume);
		}

		//Music Volume
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString(TEXT("Music Volume")));
			MusicVolume->SetDescriptionRichText(FText::FromString(TEXT("Music")));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolume->SetSliderStep(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolume->SetShouldApplyChangeSettingsImmediatly(true);

			VolumeCategoryCollection->AddChildList(MusicVolume);
		}

		//SoundFX Volume
		{
			UListDataObject_Scalar* SoundFXVolume = NewObject<UListDataObject_Scalar>();
			SoundFXVolume->SetDataID(FName("SoundFXVolume"));
			SoundFXVolume->SetDataDisplayName(FText::FromString(TEXT("SoundFX Volume")));
			SoundFXVolume->SetDescriptionRichText(FText::FromString(TEXT("SoundFX")));
			SoundFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SoundFXVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SoundFXVolume->SetSliderStep(0.01f);
			SoundFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SoundFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SoundFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			SoundFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundFXVolume));
			SoundFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundFXVolume));
			SoundFXVolume->SetShouldApplyChangeSettingsImmediatly(true);

			VolumeCategoryCollection->AddChildList(SoundFXVolume);
		}
	}

	//Sound Category
	{
		UListDataObjectCollection* SoundCategoryCollection = NewObject<UListDataObjectCollection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategory"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Sound")));

		NewAudioOptionsCollection->AddChildList(SoundCategoryCollection);

		{
			UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString(TEXT("Overall Volume")));
			AllowBackgroundAudio->SetFalseAsDefault();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplyChangeSettingsImmediatly(true);

			SoundCategoryCollection->AddChildList(AllowBackgroundAudio);
		}
	}

	RegisteredOptionsTabCollections.Add(NewAudioOptionsCollection);

}

void UOptionsDataRegistry::InitVideoTab()
{
	UListDataObjectCollection* NewVideoOptionsCollection = NewObject<UListDataObjectCollection>();
	NewVideoOptionsCollection->SetDataID(FName("VideoOptionsCollection"));
	NewVideoOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));


	//Display Category
	{
		UListDataObjectCollection* DisplayCategoryCollection = NewObject<UListDataObjectCollection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategory"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Display")));

		NewVideoOptionsCollection->AddChildList(DisplayCategoryCollection);

		FOptionDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditCondition(
			[]()->bool
			{
				const bool bIsPackagd = !GIsEditor && !GIsPlayInEditorWorld;

				return bIsPackagd;
			}
		);
		PackagedBuildOnlyCondition.SetDisableRichReason(TEXT("\n\n<Disabled>This settings can only be adjusted in a packaged buld.</>"));

		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString(TEXT("Window Mode")));
			WindowMode->SetDescriptionRichText(FText::FromString(TEXT("Change window mode")));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen,FText::FromString(TEXT("Fullscreen")));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Windowed fullscreen")));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));
			WindowMode->SetDefaultEnumValue(EWindowMode::WindowedFullscreen); 
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplyChangeSettingsImmediatly(true);

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);
			DisplayCategoryCollection->AddChildList(WindowMode);
		}
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<		UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("Screen Resolution")));
			ScreenResolution->SetDescriptionRichText(FText::FromString(TEXT("Change screen resolution")));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplyChangeSettingsImmediatly(true);

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);
			DisplayCategoryCollection->AddChildList(ScreenResolution);
		}
	}
	RegisteredOptionsTabCollections.Add(NewVideoOptionsCollection);
}

void UOptionsDataRegistry::InitControlTab()
{
	UListDataObjectCollection* NewControlOptionsCollection = NewObject<UListDataObjectCollection>();
	NewControlOptionsCollection->SetDataID(FName("ControlOptionsCollection"));
	NewControlOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionsTabCollections.Add(NewControlOptionsCollection);
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObjectBase* ObjectToSearch, TArray<UListDataObjectBase*>& OutFoundChilds) const
{
	if (!ObjectToSearch || !ObjectToSearch->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObjectBase* SubChildData : ObjectToSearch->GetAllChildListData())
	{
		if (!SubChildData)
		{
			continue;
		}

		OutFoundChilds.Add(SubChildData);

		if (SubChildData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildData, OutFoundChilds);
		}
	}
}
