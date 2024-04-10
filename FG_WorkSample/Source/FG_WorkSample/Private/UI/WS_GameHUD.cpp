// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WS_GameHUD.h"

#include "WS_GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Player/WS_BaseCharacter.h"
#include "WS_Utils.h"
//#include "Loading/KP_LoadingScreenWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameHUD, All, All);

UUserWidget* AWS_GameHUD::CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const EGameWidgetState State, const int32 ZOrder)
{
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport(ZOrder);
		/*if (State == EGameWidgetState::WaitingToStart)
		{
			const auto LoadingWidget = Cast<UWS_LoadingScreenWidget>(CurrentWidget);
			if (!LoadingWidget) return nullptr;
			LoadingWidget->FadeInAnimation();
		}*/
	}
	return CurrentWidget;
}

void AWS_GameHUD::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<UUserWidget>* GameWidgetClass = GameWidgets.Find(EGameWidgetState::WaitingToStart);
	if (GameWidgetClass && *GameWidgetClass)
	{
		CreateWidgetByClass(*GameWidgetClass, EGameWidgetState::WaitingToStart);
	}

	if (GetWorld())
	{
		const auto GameMode = Cast<AWS_GameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnGameStateChanged.AddUObject(this, &AWS_GameHUD::OnGameStateChanged);
		}
	}
}

UUserWidget* AWS_GameHUD::ShowWidget(const EGameWidgetState WidgetID, const int32 ZOrder)
{
	TSubclassOf<UUserWidget>* MenuWidgetClass = GameWidgets.Find(WidgetID);
	if (MenuWidgetClass && *MenuWidgetClass)
	{
		CreateWidgetByClass(*MenuWidgetClass, WidgetID, ZOrder);
	}
	return CurrentWidget;
}

void AWS_GameHUD::HideWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

void AWS_GameHUD::OnGameStateChanged(EGameWidgetState State)
{
	HideWidget();
	ShowWidget(State);

	UE_LOG(LogGameHUD, Display, TEXT("State: %s"), *UEnum::GetValueAsString(State));
}