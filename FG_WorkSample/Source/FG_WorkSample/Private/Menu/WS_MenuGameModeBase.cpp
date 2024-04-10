// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/WS_MenuGameModeBase.h"
#include "Menu/WS_MenuPlayerController.h"
#include "Menu/UI/WS_MenuHUD.h"

AWS_MenuGameModeBase::AWS_MenuGameModeBase()
{
    PlayerControllerClass = AWS_MenuPlayerController::StaticClass();
    HUDClass = AWS_MenuHUD::StaticClass();
}