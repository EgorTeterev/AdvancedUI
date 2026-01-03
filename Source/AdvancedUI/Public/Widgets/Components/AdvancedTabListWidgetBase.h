// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "FrontendCommonButtonBase.h"
#include "AdvancedTabListWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEDUI_API UAdvancedTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "UI Tab list settings", meta = (AllowPrivateAccess = "true",ClampMin = "1",ClampMax = "10"))
	int32 DebugPreviewTabCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Tab list settings", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "10"))
	TSubclassOf<UFrontendCommonButtonBase> TabEntryButtonWidget;

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
};
