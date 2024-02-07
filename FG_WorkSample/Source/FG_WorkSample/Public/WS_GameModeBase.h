// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WS_CoreTypes.h"
#include "WS_GameModeBase.generated.h"

UCLASS()
class FG_WORKSAMPLE_API AWS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWS_GameModeBase();

	FOnGameStateChangedSignature OnGameStateChanged;

	virtual void StartPlay() override;

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

	bool SetQuestList(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause());

	virtual bool ClearPause() override;

	void StartGame();
	void Dead();

private:

	EGameWidgetState GameState = EGameWidgetState::WaitingToStart;

	FTimerHandle GameOverTestTimer;// for test only

	void GameOver();

	void GameTimerTestUpdate(); // for test only

	void SetGameState(EGameWidgetState State);
	void ResetPlayer();
};
