// Fill out your copyright notice in the Description page of Project Settings.


#include "WS_GameModeBase.h"
#include "Player/WS_BaseCharacter.h"
#include "Player/WS_PlayerController.h"
#include "UI/WS_GameHUD.h"
#include "EngineUtils.h"
#include "WS_GameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(WS_GameModeBaseLog, All, All);

AWS_GameModeBase::AWS_GameModeBase()
{
	DefaultPawnClass = AWS_BaseCharacter::StaticClass();
	PlayerControllerClass = AWS_PlayerController::StaticClass();
	HUDClass = AWS_GameHUD::StaticClass();
}

void AWS_GameModeBase::StartPlay()
{
	Super::StartPlay();

	SetGameState(EGameWidgetState::WaitingToStart);

	StartGame();
}

bool AWS_GameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if (PauseSet)
	{
		SetGameState(EGameWidgetState::Pause);
	}

	return PauseSet;
}

bool AWS_GameModeBase::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		SetGameState(EGameWidgetState::InProgress);
	}

	return PauseCleared;
}

void AWS_GameModeBase::Dead()
{
	UE_LOG(WS_GameModeBaseLog, Display, TEXT("************ YOU ARE DEAD ************"));

	/*for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}*/

	SetGameState(EGameWidgetState::Death);
}

void AWS_GameModeBase::GameOver()
{
	if (GameState == EGameWidgetState::TheEnd) return;

	UE_LOG(WS_GameModeBaseLog, Display, TEXT("************ GAME OVER ************"));

	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

	SetGameState(EGameWidgetState::GameOver);
}

void AWS_GameModeBase::TheEnd()
{
	UE_LOG(WS_GameModeBaseLog, Display, TEXT("************ THE END ************"));

	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

	SetGameState(EGameWidgetState::TheEnd);
}

float AWS_GameModeBase::GetRemainingTime() const
{
	return GetWorldTimerManager().IsTimerActive(GameOverTimerHandle) ?
	GetWorldTimerManager().GetTimerRemaining(GameOverTimerHandle) : 0.f;
}

void AWS_GameModeBase::StartGame()
{
	UE_LOG(WS_GameModeBaseLog, Display, TEXT("************ START GAME ************"));
	SetGameState(EGameWidgetState::InProgress);

	GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &AWS_GameModeBase::GameTimerUpdate, GameTime, false);
}

void AWS_GameModeBase::GameTimerUpdate()
{
	GameOver();
	GetWorldTimerManager().ClearTimer(GameOverTimerHandle);
}

void AWS_GameModeBase::SetGameState(EGameWidgetState State)
{
	if (GameState == State) return;

	GameState = State;
	OnGameStateChanged.Broadcast(GameState);
}

void AWS_GameModeBase::ResetPlayer()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		if (It->Get() && It->Get()->GetPawn() && It->Get()->IsPlayerController())
		{
			It->Get()->GetPawn()->Reset();
			RestartPlayer(It->Get());
		}
	}
}
