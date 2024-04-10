// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WS_GameOverWidget.generated.h"

class UButton;

UCLASS()
class FG_WORKSAMPLE_API UWS_GameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
		UButton* RestartGameButton;

	virtual void NativeOnInitialized() override;

private:

	void OnGameStateChanged(EGameWidgetState NewState);

	UFUNCTION()
		void OnRestartGame();
};
