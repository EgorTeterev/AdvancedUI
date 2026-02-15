// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObjectCollection.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendFunctionLibrary.h"
#include "Tags/UIGameplayTags.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "Settings/AdvancedGameUserSettings.h"

#include "DebugHelper.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
    MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UAdvancedGameUserSettings, SetterOrGetterFuncName))


void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* OwningPlayer)
{
	InitGameplayTab();
	InitAudioTab();
	InitVideoTab();
	InitControlTab(OwningPlayer);
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

	
	UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
	WindowMode->SetDataID(FName("WindowMode"));

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

		{
			WindowMode->SetDataDisplayName(FText::FromString(TEXT("Window Mode")));
			WindowMode->SetDescriptionRichText(FText::FromString(TEXT("Change window mode")));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen,FText::FromString(TEXT("Fullscreen")));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless")));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));
			WindowMode->SetDefaultEnumValue(EWindowMode::WindowedFullscreen); 

			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));

			WindowMode->SetShouldApplyChangeSettingsImmediatly(true);

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
				[WindowMode]()->bool
				{
					const bool bIsBorderless = WindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Type::WindowedFullscreen;

					return !bIsBorderless;
				}
			);
			NonBorderlessWindowEditCondition.SetDisableRichReason(TEXT("\n\n<Disabled>This settings cant edited in 'Borderless' window mode.</>"));
			NonBorderlessWindowEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);
			ScreenResolution->AddEditCondition(NonBorderlessWindowEditCondition);

			ScreenResolution->AddEditDependencyObject(WindowMode);


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
			OverallQuality->AddIntegerOption(1,FText::FromString(TEXT("Medium")));
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
			GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
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
			ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
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
			AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
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

		//View Distance Quality
		{
			UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));

			ViewDistanceQuality->SetDataDisplayName(FText::FromString(TEXT("View Distance Quality")));
			ViewDistanceQuality->SetDescriptionRichText(FText::FromString(TEXT("View Distance Quality.")));
			ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Near")));
			ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("Far")));
			ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Very far")));
			ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinema")));

			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));

			ViewDistanceQuality->SetShouldApplyChangeSettingsImmediatly(true);

			ViewDistanceQuality->AddEditDependencyObject(OverallQuality);
			OverallQuality->AddEditDependencyObject(ViewDistanceQuality);

			GraphicsCategoryCollection->AddChildList(ViewDistanceQuality);
		}

		//Texture Quality
		{
			UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));

			TextureQuality->SetDataDisplayName(FText::FromString(TEXT("Texture Quality")));
			TextureQuality->SetDescriptionRichText(FText::FromString(TEXT("Texture Quality.")));
			TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinema")));

			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));

			TextureQuality->SetShouldApplyChangeSettingsImmediatly(true);

			TextureQuality->AddEditDependencyObject(OverallQuality);
			OverallQuality->AddEditDependencyObject(TextureQuality);

			GraphicsCategoryCollection->AddChildList(TextureQuality);
		}

		//VFX
		{
			UListDataObject_StringInteger* VisualEffectQuality = NewObject<UListDataObject_StringInteger>();
			VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));

			VisualEffectQuality->SetDataDisplayName(FText::FromString(TEXT("VFX Quality")));
			VisualEffectQuality->SetDescriptionRichText(FText::FromString(TEXT("VFX Quality.")));
			VisualEffectQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			VisualEffectQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			VisualEffectQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			VisualEffectQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			VisualEffectQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinema")));

			VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));

			VisualEffectQuality->SetShouldApplyChangeSettingsImmediatly(true);

			VisualEffectQuality->AddEditDependencyObject(OverallQuality);
			OverallQuality->AddEditDependencyObject(VisualEffectQuality);

			GraphicsCategoryCollection->AddChildList(VisualEffectQuality);
		}

		//Reflection Quality
		{
			UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));

			ReflectionQuality->SetDataDisplayName(FText::FromString(TEXT("Reflection quality")));
			ReflectionQuality->SetDescriptionRichText(FText::FromString(TEXT("Changes reflection quality.")));
			ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinema")));

			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));

			ReflectionQuality->SetShouldApplyChangeSettingsImmediatly(true);

			ReflectionQuality->AddEditDependencyObject(OverallQuality);
			OverallQuality->AddEditDependencyObject(ReflectionQuality);

			GraphicsCategoryCollection->AddChildList(ReflectionQuality);
		}

		//Post Processing Quality
		{
			UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));

			PostProcessingQuality->SetDataDisplayName(FText::FromString(TEXT("Reflection quality")));
			PostProcessingQuality->SetDescriptionRichText(FText::FromString(TEXT("Changes reflection quality.")));
			PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
			PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));

			PostProcessingQuality->SetShouldApplyChangeSettingsImmediatly(true);

			PostProcessingQuality->AddEditDependencyObject(OverallQuality);
			OverallQuality->AddEditDependencyObject(PostProcessingQuality);

			GraphicsCategoryCollection->AddChildList(PostProcessingQuality);
		}
	}

	//Advanced
	{
		UListDataObjectCollection* AdvancedCategoryCollection = NewObject<UListDataObjectCollection>();
		AdvancedCategoryCollection->SetDataID(FName("AdvancedCategoryCollection"));
		AdvancedCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Advanced")));

		VideoOptionsCollection->AddChildList(AdvancedCategoryCollection);

		//Vertical Sync
		{
			UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
			VerticalSync->SetDataID(FName("VertycalSync"));
			VerticalSync->SetDataDisplayName(FText::FromString(TEXT("V-Sync")));
			VerticalSync->SetDescriptionRichText(FText::FromString(TEXT("Sets vertical synchronization.")));

			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));

			VerticalSync->SetFalseAsDefault();
			VerticalSync->SetShouldApplyChangeSettingsImmediatly(true);
 
			FOptionDataEditConditionDescriptor FullscreenOnlyCondition;

			FullscreenOnlyCondition.SetEditCondition(
				[WindowMode]()->bool
				{
					return WindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Type::Fullscreen;
				}
			);

			FullscreenOnlyCondition.SetDisableRichReason(TEXT("\n\n<Disabled>This feature only works in Fullscreen window mode.</>"));
			FullscreenOnlyCondition.SetDisabledForcedStringValue(TEXT("False"));

			VerticalSync->AddEditCondition(FullscreenOnlyCondition);

			AdvancedCategoryCollection->AddChildList(VerticalSync);

		}

		//Frame Rate Limit
		{
			UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
			FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			FrameRateLimit->SetDataDisplayName(FText::FromString(TEXT("Frame Rate Limit")));
			FrameRateLimit->SetDescriptionRichText(FText::FromString(TEXT("Sets FPS limit.")));

			FrameRateLimit->AddDynamicOption(LexToString(30.0f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(60.0f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(90.0f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(120.0f), FText::FromString(TEXT("120 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(144.0f), FText::FromString(TEXT("144 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(165.0f), FText::FromString(TEXT("165 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(240.0f), FText::FromString(TEXT("240 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(0.0f), FText::FromString(TEXT("No limit")));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.0f));

			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));

			FrameRateLimit->SetShouldApplyChangeSettingsImmediatly(true);

			AdvancedCategoryCollection->AddChildList(FrameRateLimit);

		}

	}



	RegisteredOptionsTabCollections.Add(VideoOptionsCollection);
}

void UOptionsDataRegistry::InitControlTab(ULocalPlayer* OwningPlayer)
{
	UListDataObjectCollection* ControlOptionsCollection = NewObject<UListDataObjectCollection>();
	ControlOptionsCollection->SetDataID(FName("ControlOptionsCollection"));
	ControlOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = OwningPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);

	UEnhancedInputUserSettings* LocalUserSettings = InputSubsystem->GetUserSettings();
	check(LocalUserSettings);

	//Keyboard & Mouse Category
	{
		UListDataObjectCollection* KeyboardMouseCategory = NewObject<UListDataObjectCollection>();
		KeyboardMouseCategory->SetDataID(FName("KeyboardMouseCategory"));
		KeyboardMouseCategory->SetDataDisplayName(FText::FromString(TEXT("Keyboard & Mouse")));

		ControlOptionsCollection->AddChildList(KeyboardMouseCategory);

		//Keyboard & mouse inputs
		{
			FPlayerMappableKeyQueryOptions MouseAndKeyboardQueryOptions;
			MouseAndKeyboardQueryOptions.KeyToMatch = EKeys::S;
			MouseAndKeyboardQueryOptions.bMatchBasicKeyTypes = true;

			for (const TPair<FGameplayTag, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : LocalUserSettings->GetAllSavedKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& RowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& PlayerKeyMapping : RowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(PlayerKeyMapping, MouseAndKeyboardQueryOptions))
						{

							UListDataObject_KeyRemap* KeyData = NewObject<UListDataObject_KeyRemap>();
							KeyData->SetDataID(PlayerKeyMapping.GetMappingName());
							KeyData->SetDataDisplayName(PlayerKeyMapping.GetDisplayName());
							KeyData->InitKeyRemapData(LocalUserSettings, MappableKeyProfile, ECommonInputType::MouseAndKeyboard, PlayerKeyMapping);

							KeyboardMouseCategory->AddChildList(KeyData);
						}
					}
				}
			}
		}

		//Gamepad inputs
		{
			UListDataObjectCollection* GamepadCategory = NewObject<UListDataObjectCollection>();
			GamepadCategory->SetDataID(FName("GamepadCategory"));
			GamepadCategory->SetDataDisplayName(FText::FromString(TEXT("Gamepad")));

			ControlOptionsCollection->AddChildList(GamepadCategory);

			//Gamepad inputs
			{
				FPlayerMappableKeyQueryOptions GamepadQueryOptions;
				GamepadQueryOptions.KeyToMatch = EKeys::Gamepad_FaceButton_Top;
				GamepadQueryOptions.bMatchBasicKeyTypes = true;

				for (const TPair<FGameplayTag, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : LocalUserSettings->GetAllSavedKeyProfiles())
				{
					UEnhancedPlayerMappableKeyProfile* MappableGamepadProfile = ProfilePair.Value;
					check(MappableGamepadProfile);

					for (const TPair<FName, FKeyMappingRow>& RowPair : MappableGamepadProfile->GetPlayerMappingRows())
					{
						for (const FPlayerKeyMapping& PlayerKeyMapping : RowPair.Value.Mappings)
						{
							if (MappableGamepadProfile->DoesMappingPassQueryOptions(PlayerKeyMapping, GamepadQueryOptions))
							{

								UListDataObject_KeyRemap* GamepadButtonData = NewObject<UListDataObject_KeyRemap>();
								GamepadButtonData->SetDataID(PlayerKeyMapping.GetMappingName());
								GamepadButtonData->SetDataDisplayName(PlayerKeyMapping.GetDisplayName());
								GamepadButtonData->InitKeyRemapData(LocalUserSettings, MappableGamepadProfile, ECommonInputType::Gamepad, PlayerKeyMapping);

								GamepadCategory->AddChildList(GamepadButtonData);
							}
						}
					}
				}
			}
		}


	}

	RegisteredOptionsTabCollections.Add(ControlOptionsCollection);
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
