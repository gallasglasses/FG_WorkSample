// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WS_TheEndWidget.h"
#include "WS_GameModeBase.h"
#include "WS_CoreTypes.h"


void UWS_TheEndWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetWorld())
    {
        const auto GameMode = Cast<AWS_GameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnGameStateChanged.AddUObject(this, &UWS_TheEndWidget::OnGameStateChanged);
        }
    }
}

void UWS_TheEndWidget::OnGameStateChanged(EGameWidgetState NewState)
{
    if (NewState == EGameWidgetState::TheEnd)
    {
        //
    }
}
