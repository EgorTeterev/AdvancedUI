// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/AdvancedActivatableWidget.h"
#include "Types/AdvancedUIEnumTypes.h"
#include "ConfirmScreenWidget.generated.h"

class UCommonTextBlock;
class UDynamicEntryBox;

USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ButtonType = EConfirmScreenButtonType::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonText;
};


UCLASS()
class ADVANCEDUI_API UConfirmInfoObject : public UObject
{
	GENERATED_BODY()

public:
	
	static UConfirmInfoObject* CreateOKScreen(const FText& ScreenTitle,const FText& ScreenMessage);
	static UConfirmInfoObject* CreateYesOrNoScreen(const FText& ScreenTitle, const FText& ScreenMessage);
	static UConfirmInfoObject* CreateOkOrCancelScreen(const FText& ScreenTitle, const FText& ScreenMessage);

	UPROPERTY(Transient)
	FText ScreenTitle;

	UPROPERTY(Transient)
	FText ScreenMessage;

	UPROPERTY(Transient)
	TArray<FConfirmScreenButtonInfo> AvailableScreenButtons;
};


/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UConfirmScreenWidget : public UAdvancedActivatableWidget
{
	GENERATED_BODY()
	

public:
	// 
	void InitComfirmScreen(UConfirmInfoObject* ScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback);

private:
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ConfirmScreenTitle;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* ConfirmScreenMessage;

	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* DynamicEntryBoxButtons;



};
