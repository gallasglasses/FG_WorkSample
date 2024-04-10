// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WS_GamePauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"


bool UWS_GamePauseWidget::Initialize()
{
	const auto InitStatus = Super::Initialize();

	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &UWS_GamePauseWidget::OnClearPause);
	}

	return InitStatus;
}

void UWS_GamePauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	
	GetWorld()->GetAuthGameMode()->ClearPause();
}