// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WS_HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UWS_HealthBarWidget::SetHealthPercent(float Percent)
{
	if (!HealthProgressBar)
	{
		return;
	}

	const auto HealthProgressBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent))
		? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	HealthProgressBar->SetVisibility(HealthProgressBarVisibility);

	const auto HealthProgressBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
	HealthProgressBar->SetFillColorAndOpacity(HealthProgressBarColor);

	HealthProgressBar->SetPercent(Percent);

	/*if (FMath::IsNearlyZero(Percent))
	{
		StatusImage->SetVisibility(ESlateVisibility::Hidden);
	}*/
}

void UWS_HealthBarWidget::SetScaleDamage(float Damage)
{
	if (Damage < 0.0f)
	{
		ScaleDamage->SetVisibility(ESlateVisibility::Visible);
		ScaleDamage->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), -Damage)));
		OnTakeDamage();
		PlayAnimation(OnTakeDamageAnim);
	}
	else
	{
		ScaleDamage->SetVisibility(ESlateVisibility::Hidden);
	}
}
