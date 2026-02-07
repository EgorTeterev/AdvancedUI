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
	UListDataObjectCollection* VideoOptionsCollection = NewObject<UListDataObjectCollection>();
	VideoOptionsCollection->SetDataID(FName("VideoOptionsCollection"));
	VideoOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));


	//Display Category
	{
		UListDataObjectCollection* DisplayCategoryCollection = NewObject<UListDataObjectCollection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategory"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Display")));

		VideoOptionsCollection->AddChildList(DisplayCategoryCollection);

		FOptionDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditCondition(
			[]()->bool
			{
				const bool bIsPackagd = !GIsEditor && !GIsPlayInEditorWorld;

				return bIsPackagd;
			}
		);
		PackagedBuildOnlyCondition.SetDisableRichReason(TEXT("\n\n<Disabled>This settings can only be adjusted in a packaged buld.</>"));

		UListDataObject_StringEnum* CachedWindowMode = nullptr;
		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString(TEXT("Window Mode")));
			WindowMode->SetDescriptionRichText(FText::FromString(TEXT("Change window mode")));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen,FText::FromString(TEXT("Fullscreen")));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless")));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));
			WindowMode->SetDefaultEnumValue(EWindowMode::WindowedFullscreen); 
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplyChangeSettingsImmediatly(true);

			CachedWindowMode = WindowMode;

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);
			DisplayCategoryCollection->AddChildList(WindowMode);
		}


		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("Screen Resolution")));
			ScreenResolution->SetDescriptionRichText(FText::FromString(TEXT("Change screen resolution")));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplyChangeSettingsImmediatly(true);

			FOptionDataEditConditionDescriptor NonBorderlessWindowEditCondition;
			NonBorderlessWindowEditCondition.SetEditCondition(
				[CachedWindowMode]()->bool
				{
					const bool bIsBorderless = CachedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Type::WindowedFullscreen;

					return !bIsBorderless;
				}
			);
			NonBorderlessWindowEditCondition.SetDisableRichReason(TEXT("\n\n<Disabled>This settings cant edited in 'Borderless' window mode.</>"));
			NonBorderlessWindowEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);
			ScreenResolution->AddEditCondition(NonBorderlessWindowEditCondition);

			ScreenResolution->AddEditDependencyObject(CachedWindowMode);


			DisplayCategoryCollection->AddChildList(ScreenResolution);
		}
	}

	//Graphics Category
	{
		UListDataObjectCollection* GraphicsCategoryCollection = NewObject<UListDataObjectCollection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategory"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Graphics")));

		VideoOptionsCollection->AddChildList(GraphicsCategoryCollection);

		//Display Gamma
		{
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(FText::FromString(TEXT("Brightness")));
			DisplayGamma->SetDescriptionRichText(FText::FromString(TEXT("Change brightness")));
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f,2.7f)); // Unreal engine default value is 2.2.
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));

			GraphicsCategoryCollection->AddChildList(DisplayGamma);
		}


		UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
		OverallQuality->SetDataID(FName("OverallQuality"));

		//Overall Quality
		{
			OverallQuality->SetDataDisplayName(FText::FromString(TEXT("Overall Quality")));
			OverallQuality->SetDescriptionRichText(FText::FromString(TEXT("Changes other quality options.")));
			OverallQuality->AddIntegerOption(0,FText::FromString(TEXT("Low")));
			OverallQuality->AddIntegerOption(1,FText::FromString(TEXT("Mid")));
			OverallQuality->AddIntegerOption(2,FText::FromString(TEXT("High")));
			OverallQuality->AddIntegerOption(3,FText::FromString(TEXT("Epic")));
			OverallQuality->AddIntegerOption(4,FText::FromString(TEXT("Cinema")));

			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			
			OverallQuality->SetShouldApplyChangeSettingsImmediatly(true);

			GraphicsCategoryCollection->AddChildList(OverallQuality);
		}

		//Resolution Scale
		{
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();

			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(FText::FromString(TEXT("Resolution Scale")));
			ResolutionScale->SetDescriptionRichText(FText::FromString(TEXT("Changes resolution.")));

			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f,1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.1f,1.f));
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));

			ResolutionScale->SetShouldApplyChangeSettingsImmediatly(true);

			ResolutionScale->AddEditDependencyObject(OverallQuality);

			GraphicsCategoryCollection->AddChildList(ResolutionScale);
		}

		//Global Illumination
		{
			UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();

			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDataDisplayName(FText::FromString(TEXT("Overall Quality")));
			GlobalIlluminationQuality->SetDescriptionRichText(FText::FromString(TEXT("Global illumination.")));
			GlobalIlluminationQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("Mid")));
			GlobalIlluminationQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			GlobalIlluminationQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			GlobalIlluminationQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinema")));

			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));

			GlobalIlluminationQuality->SetShouldApplyChangeSettingsImmediatly(true);

			GlobalIlluminationQuality->AddEditDependencyObject(OverallQuality);
			OverallQuality->AddEditDependencyObject(GlobalIlluminationQuality);

			GraphicsCategoryCollection->AddChildList(GlobalIlluminationQuality);
		}

		//Shadow Quality
		{
			UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));

			ShadowQuality->SetDataDisplayName(FText::FromString(TEXT("Shadow Quality")));
			ShadowQuality->SetDescriptionRichText(FText::FromString(TEXT("Shadow quality.")));
			ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Mid")));
			ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinema")));

			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));

			ShadowQuality->SetShouldApplyChangeSettingsImmediatly(true);

			ShadowQuality->AddEditDependencyObject(OverallQuality);
			OverallQuality->AddEditDependencyObject(ShadowQuality);

			GraphicsCategoryCollection->AddChildList(ShadowQuality);
		}

		//Anti Aliasing
		{
			UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));

			AntiAliasingQuality->SetDataDisplayName(FText::FromString(TEXT("Anti-AliasingQuality")));
			AntiAliasingQuality->SetDescriptionRichText(FText::FromString(TEXT("Anti Aliasing Quality.")));
			AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Mid")));
			AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinema")));

			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));

			AntiAliasingQuality->SetShouldApplyChangeSettingsImmediatly(true);

			AntiAliasingQuality->AddEditDependencyObject(OverallQuality);
			OverallQuality->AddEditDependencyObject(AntiAliasingQuality);

			GraphicsCategoryCollection->AddChildList(AntiAliasingQuality);
		}


	}

	RegisteredOptionsTabCollections.Add(VideoOptionsCollection);
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
