// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WS_GameTimeWidget.h"
#include "WS_GameModeBase.h"


float UWS_GameTimeWidget::GetSecondsRemaining() const
{
	const auto GameMode = GetWSGameMode();
	return GameMode ? GameMode->GetRemainingTime() : 0.f;
}

float UWS_GameTimeWidget::GetGameTime() const
{
	const auto GameMode = GetWSGameMode();
	return GameMode ? GameMode->GetGameTime() : 0.f;
}

AWS_GameModeBase* UWS_GameTimeWidget::GetWSGameMode() const
{
	return GetWorld() ? Cast<AWS_GameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}