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

	virtual bool ClearPause() override;

	void StartGame();
	void Dead();

	UFUNCTION(BlueprintCallable)
		void GameOver();

	UFUNCTION(BlueprintCallable)
		void TheEnd();

	UFUNCTION(BlueprintCallable)
		float GetRemainingTime() const;

	UFUNCTION(BlueprintCallable)
		float GetGameTime() const { return GameTime; };

private:

	EGameWidgetState GameState = EGameWidgetState::WaitingToStart;

	FTimerHandle GameOverTimerHandle;

	float GameTime = 420.f;

	void GameTimerUpdate();

	void SetGameState(EGameWidgetState State);
	void ResetPlayer();
};
