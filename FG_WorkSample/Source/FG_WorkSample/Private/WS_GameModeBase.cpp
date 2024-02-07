// Fill out your copyright notice in the Description page of Project Settings.


#include "WS_GameModeBase.h"
#include "Player/WS_BaseCharacter.h"
#include "Player/WS_PlayerController.h"
#include "UI/WS_GameHUD.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(KProjectGameModeBaseLog, All, All);

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

	//SpawnBots();

	StartGame();
	//SetGameState(EGameWidgetState::InProgress);
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

bool AWS_GameModeBase::SetQuestList(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if (PauseSet)
	{
		SetGameState(EGameWidgetState::QuestList);
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
	UE_LOG(KProjectGameModeBaseLog, Display, TEXT("************ YOU ARE DEAD ************"));

	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

	SetGameState(EGameWidgetState::Death);
}

void AWS_GameModeBase::GameOver()
{
	UE_LOG(KProjectGameModeBaseLog, Display, TEXT("************ GAME OVER ************"));

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

void AWS_GameModeBase::StartGame()
{
	UE_LOG(KProjectGameModeBaseLog, Display, TEXT("************ START GAME ************"));
	SetGameState(EGameWidgetState::InProgress);


	// TODO: trigger for game over
	//GetWorldTimerManager().SetTimer(GameOverTestTimer, this, &AKProjectGameModeBase::GameTimerTestUpdate, 30.f, false); // for test
}

void AWS_GameModeBase::GameTimerTestUpdate()
{
	GameOver();
	GetWorldTimerManager().ClearTimer(GameOverTestTimer);
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
