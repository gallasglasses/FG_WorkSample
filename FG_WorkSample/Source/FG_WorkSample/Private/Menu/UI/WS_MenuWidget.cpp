// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/UI/WS_MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "WS_CoreTypes.h"
#include "WS_GameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(MenuWidgetLog, All, All);

void UWS_MenuWidget::ShowOpening()
{
	PlayAnimation(OpeningAnimation);
}

void UWS_MenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UWS_MenuWidget::OnStartGame);
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UWS_MenuWidget::OnQuitGame);
	}
}

void UWS_MenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation != EndingAnimation) return; 
	
	if (!GetWorld()) return;
	const auto GameInstance = GetWorld()->GetGameInstance<UWS_GameInstance>();
	if (!GameInstance) return;

	if (GameInstance->GetLevel(EGameLevel::StartLevel).IsNull())
	{
		UE_LOG(MenuWidgetLog, Error, TEXT("Level name is NONE"));
		return;
	}

	UGameplayStatics::OpenLevel(this, FName(GameInstance->GetLevel(EGameLevel::StartLevel).ToSoftObjectPath().GetAssetName()));
}

void UWS_MenuWidget::OnStartGame()
{
	PlayAnimation(EndingAnimation);
}

void UWS_MenuWidget::OnQuitGame()
{
	UE_LOG(MenuWidgetLog, Display, TEXT("************** OnQuitGame *************"));
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}