// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/AdvancedTabListWidgetBase.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Editor/WidgetCompilerLog.h"



void UAdvancedTabListWidgetBase::RequestRegisterTab(const FName& TabID, const FText& TabDisplayName)
{


}



#if WITH_EDITOR
void UAdvancedTabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	
	if (!TabEntryButtonWidget)
	{
		CompileLog.Error(FText::FromString(TEXT("TabEntryButtonWidget is not specified") + GetClass()->GetName() + TEXT(" needs valid widget class")));
	}
}
#endif