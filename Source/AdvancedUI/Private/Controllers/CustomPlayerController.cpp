// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/CustomPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void ACustomPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	//find main menu camera
	TArray<AActor*> FoundedCameras;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("MainMenuCamera"), FoundedCameras);

	if (FoundedCameras.IsEmpty())
	{
		return;
	}

	SetViewTarget(FoundedCameras[0]);
}