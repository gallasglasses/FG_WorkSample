// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WS_TheEndWidget.generated.h"

UCLASS()
class FG_WORKSAMPLE_API UWS_TheEndWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

private:

	void OnGameStateChanged(EGameWidgetState NewState);

};
