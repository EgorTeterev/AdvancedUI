// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UILoadingScreenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UUILoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonChangedDelegate, const FString&, CureentLoadingReason);

	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonChangedDelegate OnLoadingReasonChanged;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;

private:
	void OnMapPreloaded(const FWorldContext& WorldContext, const FString& MapName);
	void OnMapPostLoaded(UWorld* LoadedWorld);
	
	void TryUpdateLoadingScreen();
	void TryDisplayLoadingScreen();
	void TryRemoveLoadingScreen();
	void NotifyLoadingScreenVisibilityChanged(bool bIsVisible);

	bool CheckTheNeedToShowLoadingScreen();
	bool ShouldShowLoadingScreen();
	bool IsLoadingScreenActive() const;
	bool bIsCurrentlyLoadingMap = false;
	float HoldLoadingScreenStartUpTime = -1.f;
	FString CurrentLoadingReason;

	TSharedPtr<SWidget> CachedCreatedLoadingScreen;
};
