// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ObjectDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObjectCollection.h"


void UObjectDataRegistry::InitOptionsDataRegistry(ULocalPlayer* OwningPlayer)
{
	InitGameplayTab();
	InitAudioTab();
	InitVideoTab();
	InitControlTab();
}

void UObjectDataRegistry::InitGameplayTab()
{
	UListDataObjectCollection* NewGameplayOptionsCollection = NewObject<UListDataObjectCollection>();
	NewGameplayOptionsCollection->SetDataId(FName("GameplayOptionsCollection"));
	NewGameplayOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));

	RegisteredOptionsTabCollections.Add(NewGameplayOptionsCollection);
}

void UObjectDataRegistry::InitAudioTab()
{
	UListDataObjectCollection* NewAudioOptionsCollection = NewObject<UListDataObjectCollection>();
	NewAudioOptionsCollection->SetDataId(FName("AudioOptionsCollection"));
	NewAudioOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	RegisteredOptionsTabCollections.Add(NewAudioOptionsCollection);

}

void UObjectDataRegistry::InitVideoTab()
{
	UListDataObjectCollection* NewVideoOptionsCollection = NewObject<UListDataObjectCollection>();
	NewVideoOptionsCollection->SetDataId(FName("VideoOptionsCollection"));
	NewVideoOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));

	RegisteredOptionsTabCollections.Add(NewVideoOptionsCollection);
}

void UObjectDataRegistry::InitControlTab()
{
	UListDataObjectCollection* NewControlOptionsCollection = NewObject<UListDataObjectCollection>();
	NewControlOptionsCollection->SetDataId(FName("ControlOptionsCollection"));
	NewControlOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionsTabCollections.Add(NewControlOptionsCollection);
}
