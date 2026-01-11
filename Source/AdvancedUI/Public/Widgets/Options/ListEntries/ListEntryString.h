// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/ListEntryBase.h"
#include "ListEntryString.generated.h"

class UFrontendCommonButtonBase;
class UAdvancedCommonRotator;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UListEntryString : public UListEntryBase
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget,AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* PreviousOptionCommonButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UAdvancedCommonRotator* OptionCommonRotator;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* NextOptionCommonButton;


};
