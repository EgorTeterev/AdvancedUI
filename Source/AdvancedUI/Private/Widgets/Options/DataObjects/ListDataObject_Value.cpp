// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_Value.h"

void UListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper> OtherDynamicSetter)
{
	DataDynamicSetter = OtherDynamicSetter;
}

void UListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper> OtherDynamicGetter)
{
	DataDynamicGetter = OtherDynamicGetter;
}
