// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WS_CoreTypes.h"
#include "WS_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class FG_WORKSAMPLE_API AWS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AWS_PlayerController();

protected:

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputMappingContext* BaseCharacterContext;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* PauseGameAction;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent();
	void OnPauseGame();

private:

	void OnGameStateChanged(EGameWidgetState State);
	void OnDead();
};
