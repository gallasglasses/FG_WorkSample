// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WS_GameOverWidget.h"
#include "WS_GameModeBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "WS_CoreTypes.h"


void UWS_GameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld())
    {
        const auto GameMode = Cast<AWS_GameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnGameStateChanged.AddUObject(this, &UWS_GameOverWidget::OnGameStateChanged);
        }
    }

    if (RestartGameButton)
    {
        RestartGameButton->OnClicked.AddDynamic(this, &UWS_GameOverWidget::OnRestartGame);
    }
}

void UWS_GameOverWidget::OnGameStateChanged(EGameWidgetState NewState)
{
    if (NewState == EGameWidgetState::GameOver)
    {
        //
    }
}

void UWS_GameOverWidget::OnRestartGame()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}