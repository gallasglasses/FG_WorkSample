// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WS_InteractWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class FG_WORKSAMPLE_API UWS_InteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetTimePercent(float CurrentTime);
	void SetMaxHoldingTime(float HoldingTimeAmount) { MaxHoldingTime = HoldingTimeAmount; };

protected:

		UPROPERTY(meta = (BindWidget))
			UProgressBar* TimeDependantProgressBar;

		UPROPERTY(meta = (BindWidget))
			UTextBlock* InteractMessageText;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
			float MaxHoldingTime = 0.f;

private:

	float CurrentHoldingTime = 0.f;
};
