// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WS_PlayerController.h"

#include "Player/WS_BaseCharacter.h"
#include "UI/WS_GameHUD.h"
#include "WS_GameModeBase.h"
#include "GameFramework/GameModeBase.h"
#include "EnhancedInputComponent.h"

DEFINE_LOG_CATEGORY_STATIC(WS_PlayerControllerLog, All, All);

AWS_PlayerController::AWS_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		const auto GameMode = Cast<AWS_GameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnGameStateChanged.AddUObject(this, &AWS_PlayerController::OnGameStateChanged);
		}

		const auto PlayerPawn = Cast<AWS_BaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (PlayerPawn)
		{
			UE_LOG(WS_PlayerControllerLog, Display, TEXT("OnDead.AddUObject this, &AWS_PlayerController::OnDead"));
			PlayerPawn->OnDead.AddUObject(this, &AWS_PlayerController::OnDead);
		}
	}
}

void AWS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &AWS_PlayerController::OnPauseGame);
	}
}

void AWS_PlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	const auto GameMode = Cast<AWS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->SetPause(this);
	}

	//TODO: Find and add some situations for check to pause/unpause. 
	/**
	 * SetPause: CanUnpauseDelegate
	 * Adds the delegate to the list if the player Controller has the right to pause
	 * the game. The delegate is called to see if it is ok to unpause the game, e.g.
	 * the reason the game was paused has been cleared.
	 * @param PC the player Controller to check for admin privs
	 * @param CanUnpauseDelegate the delegate to query when checking for unpause
	 */
}

void AWS_PlayerController::OnDead()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	const auto GameMode = Cast<AWS_GameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->Dead();
	}
}

void AWS_PlayerController::OnGameStateChanged(EGameWidgetState State)
{
	if (State == EGameWidgetState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}
}