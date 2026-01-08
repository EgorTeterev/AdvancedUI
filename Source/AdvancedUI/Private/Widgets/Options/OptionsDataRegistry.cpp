// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObjectCollection.h"


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

	return FoundTabCollection->GetAllChildListData();
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

		NewGameplayOptionsCollection->AddChildList(GameDifficulty);
	}



	RegisteredOptionsTabCollections.Add(NewGameplayOptionsCollection);
}

void UOptionsDataRegistry::InitAudioTab()
{
	UListDataObjectCollection* NewAudioOptionsCollection = NewObject<UListDataObjectCollection>();
	NewAudioOptionsCollection->SetDataID(FName("AudioOptionsCollection"));
	NewAudioOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	RegisteredOptionsTabCollections.Add(NewAudioOptionsCollection);

}

void UOptionsDataRegistry::InitVideoTab()
{
	UListDataObjectCollection* NewVideoOptionsCollection = NewObject<UListDataObjectCollection>();
	NewVideoOptionsCollection->SetDataID(FName("VideoOptionsCollection"));
	NewVideoOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));

	RegisteredOptionsTabCollections.Add(NewVideoOptionsCollection);
}

void UOptionsDataRegistry::InitControlTab()
{
	UListDataObjectCollection* NewControlOptionsCollection = NewObject<UListDataObjectCollection>();
	NewControlOptionsCollection->SetDataID(FName("ControlOptionsCollection"));
	NewControlOptionsCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionsTabCollections.Add(NewControlOptionsCollection);
}
