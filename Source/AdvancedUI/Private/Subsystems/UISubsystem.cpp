// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UISubsystem.h"


UUISubsystem* UUISubsystem::Get()
{
	return nullptr;
}

bool UUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UUISubsystem::RegisterPrimeLayoutWidget(UPrimaryLayoutWidget* LayoutWidgetToSet)
{
	check(LayoutWidgetToSet);
	LayoutWidget = LayoutWidgetToSet;
}

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UUISubsystem::Deinitialize()
{

}
