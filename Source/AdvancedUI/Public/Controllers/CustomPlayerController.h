// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDUI_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	//~Begin PlayerController Interface
	virtual void OnPossess(APawn* PawnToPossess) override;
	//~End PlayerController Interface
};
