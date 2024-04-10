// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WS_InteractWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWS_InteractWidget::SetTimePercent(float CurrentTime)
{
	if (!TimeDependantProgressBar)
	{
		return;
	}
	CurrentTime <= 0.f ? TimeDependantProgressBar->SetPercent(0.f) :
						TimeDependantProgressBar->SetPercent(CurrentTime/MaxHoldingTime);
}
