// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/WS_MenuPlayerController.h"
#include "WS_GameInstance.h"

void AWS_MenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
