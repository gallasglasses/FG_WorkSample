// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WS_CoreTypes.h"
#include "WS_GameHUD.generated.h"

UCLASS()
class FG_WORKSAMPLE_API AWS_GameHUD : public AHUD
{
	GENERATED_BODY()

protected:

	UUserWidget* CreateWidgetByClass(const TSubclassOf<UUserWidget> GameWidgetClass, const EGameWidgetState State, const int32 ZOrder = 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TMap<EGameWidgetState, TSubclassOf<UUserWidget>> GameWidgets;

	virtual void BeginPlay() override;

	UPROPERTY()
		UUserWidget* CurrentWidget;

public:

	UFUNCTION(BlueprintCallable)
		UUserWidget* GetCurrentWidget() const { return CurrentWidget; };

	UFUNCTION(BlueprintCallable)
		UUserWidget* ShowWidget(const EGameWidgetState WidgetID, const int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable)
		void HideWidget();

private:

	void OnGameStateChanged(EGameWidgetState State);
};
