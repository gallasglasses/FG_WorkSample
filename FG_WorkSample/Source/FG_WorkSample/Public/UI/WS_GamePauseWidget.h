// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WS_GamePauseWidget.generated.h"

class UButton;

UCLASS()
class FG_WORKSAMPLE_API UWS_GamePauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

protected:

	UPROPERTY(meta = (BindWidget))
		UButton* ClearPauseButton;

private: 

	UFUNCTION()
		void OnClearPause();
};
