// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Settings/AdvancedGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "DebugHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);

	AvailableResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B) -> bool
		{
			return A.SizeSquared() > B.SizeSquared();
		}
	);

	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}

	MaximumAllowedResolution = ResToValueString(AvailableResolutions[0]);
	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(MaximumAllowedResolution))
	{
		CurrentDisplayText = ResToDisplayText(UAdvancedGameUserSettings::Get()->GetScreenResolution());
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& Resolution) const
{
	return FString::Printf(TEXT("(X=%i,Y=%i)"),Resolution.X,Resolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& Resolution) const
{
	const FString DisplayString = FString::Printf(TEXT("%i x %i"), Resolution.X, Resolution.Y);
	return FText::FromString(DisplayString);
}
