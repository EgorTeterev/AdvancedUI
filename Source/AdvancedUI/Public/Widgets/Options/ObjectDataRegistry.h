// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObjectDataRegistry.generated.h"

class UListDataObjectCollection;
/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UObjectDataRegistry : public UObject
{
	GENERATED_BODY()
	
public:
	void InitOptionsDataRegistry(ULocalPlayer* OwningPlayer);

	const TArray<UListDataObjectCollection*> GetRegisteredOptionCollections() const { return RegisteredOptionsTabCollections; };

private:
	void InitGameplayTab();
	void InitAudioTab();
	void InitVideoTab();
	void InitControlTab();

	UPROPERTY(Transient)
	TArray<UListDataObjectCollection*> RegisteredOptionsTabCollections;

};
