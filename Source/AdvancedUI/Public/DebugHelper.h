// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


namespace AUIDebug
{
	static void ConsoleMessage(const FString& Msg, int32 Key = 1, const FColor& Color = FColor::Red)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Key, 1.0f, Color, Msg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}
}