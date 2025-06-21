// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/PrimaryLayoutWidget.h"
#include "UISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UUISubsystem* Get();


	//~Begin Subsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~End Subsystem Interface

	UFUNCTION(BlueprintCallable)
	void RegisterPrimeLayoutWidget(UPrimaryLayoutWidget* LayoutWidgetToSet);
private:
	UPROPERTY(Transient)
	UPrimaryLayoutWidget* LayoutWidget;
};
