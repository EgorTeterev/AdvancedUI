// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/ListEntryBase.h"
#include "ListEntryKeyRemap.generated.h"

class UFrontendCommonButtonBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UListEntryKeyRemap : public UListEntryBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget,AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButtonKeyRemap;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButtonKeyReset;
};
