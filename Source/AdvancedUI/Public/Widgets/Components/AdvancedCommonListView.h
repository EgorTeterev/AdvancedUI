// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "AdvancedCommonListView.generated.h"

class UDA_DataListEntryMapping;
/**
 * 
 */
UCLASS()
class ADVANCEDUI_API UAdvancedCommonListView : public UCommonListView
{
	GENERATED_BODY()
	
protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;


private:

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif

	UPROPERTY(EditAnywhere,Category = "List view settings")
	UDA_DataListEntryMapping* DataListEntryMapping;
};
