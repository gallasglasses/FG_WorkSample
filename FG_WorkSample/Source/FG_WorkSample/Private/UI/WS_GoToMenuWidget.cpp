// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WS_GoToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "WS_GameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(GoToMenuWidgetLog, All, All);

void UWS_GoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GoToMenuButton)
	{
		GoToMenuButton->OnClicked.AddDynamic(this, &UWS_GoToMenuWidget::OnGoToMainMenu);
	}
}

void UWS_GoToMenuWidget::OnGoToMainMenu()
{
	if (!GetWorld()) return;

	const auto GameInstance = GetWorld()->GetGameInstance<UWS_GameInstance>();
	if (!GameInstance) return;

	if (GameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(GoToMenuWidgetLog, Error, TEXT("Menu Level name is NONE"));
		return;
	}
	UE_LOG(GoToMenuWidgetLog, Display, TEXT("************ Quit To Main Menu ************"));
	UGameplayStatics::OpenLevel(this, GameInstance->GetMenuLevelName());
}
