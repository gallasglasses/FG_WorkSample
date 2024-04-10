// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WS_PlayerUIWidget.generated.h"

class UProgressBar;
class UImage;

UCLASS()
class FG_WORKSAMPLE_API UWS_PlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetStaminaPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetManaPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
		void OnTakeDamage();

	void SetHealthPercent(float HealthPercent);

	void SetStaminaPercent(float StaminaPercent);

	void SetManaPercent(float ManaPercent);

	virtual bool Initialize() override;

protected:
	
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthProgressBar;
	
	UPROPERTY(meta = (BindWidget))
		UProgressBar* ManaProgressBar;
	
	UPROPERTY(meta = (BindWidget))
		UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
		UImage* StatusImage;

	UPROPERTY(meta = (BindWidget))
		UImage* DamageImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* DamageAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor HealthProgressBarColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor ManaProgressBarColor = FLinearColor::Blue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		FLinearColor StaminaProgressBarColor = FLinearColor::Yellow;

	virtual void NativePreConstruct() override;

private:

	void OnHealthChanged(float Health, float HealthDelta);
	void OnStaminaChanged(float Stamina, float StaminaDelta);
	void OnManaChanged(float Mana, float ManaDelta);
};
