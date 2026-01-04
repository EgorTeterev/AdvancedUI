// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObjectCollection.h"


void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* OwningPlayer)
{
	InitGameplayTab();
	InitAudioTab();
	InitVideoTab();
	InitControlTab();
}

void UOptionsDataRegistry::InitGameplayTab()
{
	UListDataObjectCollection* NewGameplayOptionsCollection = NewObject<UListDataObjectCollection>();
	NewGameplayOptionsCollection->SetDataId(FName("GameplayOptionsCollection"));
	NewGameplayOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));

	RegisteredOptionsTabCollections.Add(NewGameplayOptionsCollection);
}

void UOptionsDataRegistry::InitAudioTab()
{
	UListDataObjectCollection* NewAudioOptionsCollection = NewObject<UListDataObjectCollection>();
	NewAudioOptionsCollection->SetDataId(FName("AudioOptionsCollection"));
	NewAudioOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	RegisteredOptionsTabCollections.Add(NewAudioOptionsCollection);

}

void UOptionsDataRegistry::InitVideoTab()
{
	UListDataObjectCollection* NewVideoOptionsCollection = NewObject<UListDataObjectCollection>();
	NewVideoOptionsCollection->SetDataId(FName("VideoOptionsCollection"));
	NewVideoOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));

	RegisteredOptionsTabCollections.Add(NewVideoOptionsCollection);
}

void UOptionsDataRegistry::InitControlTab()
{
	UListDataObjectCollection* NewControlOptionsCollection = NewObject<UListDataObjectCollection>();
	NewControlOptionsCollection->SetDataId(FName("ControlOptionsCollection"));
	NewControlOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionsTabCollections.Add(NewControlOptionsCollection);
}
