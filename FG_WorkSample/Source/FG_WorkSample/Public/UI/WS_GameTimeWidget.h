// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WS_GameTimeWidget.generated.h"

class AWS_GameModeBase;

UCLASS()
class FG_WORKSAMPLE_API UWS_GameTimeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetSecondsRemaining() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetGameTime() const;

private:

	AWS_GameModeBase* GetWSGameMode() const;
};
