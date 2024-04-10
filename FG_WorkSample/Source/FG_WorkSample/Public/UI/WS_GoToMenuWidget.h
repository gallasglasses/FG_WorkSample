// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WS_GoToMenuWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class FG_WORKSAMPLE_API UWS_GoToMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
		UButton* GoToMenuButton;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* GoToMenuText;

private:

	UFUNCTION()
		void OnGoToMainMenu();
};
