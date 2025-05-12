// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WS_HealthBarWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UWidgetAnimation;

UCLASS()
class FG_WORKSAMPLE_API UWS_HealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
		void OnTakeDamage();

	void SetHealthPercent(float Percent);

	void SetScaleDamage(float Damage);

protected:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ScaleDamage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	 	UWidgetAnimation* OnTakeDamageAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		float PercentVisibilityThreshold = 0.8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		float PercentColorThreshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor GoodColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor BadColor = FLinearColor::Red;
};
