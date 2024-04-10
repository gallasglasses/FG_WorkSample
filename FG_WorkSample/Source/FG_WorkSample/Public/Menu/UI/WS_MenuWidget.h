// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WS_MenuWidget.generated.h"

class UButton;
class UWidgetAnimation;

UCLASS()
class FG_WORKSAMPLE_API UWS_MenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void ShowOpening();

protected:

	UPROPERTY(meta = (BindWidget))
		UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
		UButton* QuitGameButton;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* OpeningAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* EndingAnimation;

	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);

private:

	UFUNCTION()
		void OnStartGame();

	UFUNCTION()
		void OnQuitGame();
	
};
