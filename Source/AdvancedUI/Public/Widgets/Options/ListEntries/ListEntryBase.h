// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ListEntryBase.generated.h"

class UCommonTextBlock;
/**
 *
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UListEntryBase : public UCommonUserWidget , public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidgetOptional,AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonTextDisplayName;
};
