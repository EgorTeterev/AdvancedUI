// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObjectCollection;
class UListDataObjectBase;
/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()
	
public:
	void InitOptionsDataRegistry(ULocalPlayer* OwningPlayer);

	const TArray<UListDataObjectCollection*> GetRegisteredOptionCollections() const { return RegisteredOptionsTabCollections; };

	TArray<UListDataObjectBase*> GetListSourceItemsBySelectedTabID(const FName& SelectedTabID) const;

private:
	void InitGameplayTab();
	void InitAudioTab();
	void InitVideoTab();
	void InitControlTab();

	UPROPERTY(Transient)
	TArray<UListDataObjectCollection*> RegisteredOptionsTabCollections;

};
