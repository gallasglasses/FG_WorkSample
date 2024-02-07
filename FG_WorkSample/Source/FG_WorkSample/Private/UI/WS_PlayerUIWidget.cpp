// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WS_PlayerUIWidget.h"

#include "Components/WS_HealthComponent.h"
#include "Components/WS_StaminaComponent.h"
#include "Components/WS_ManaComponent.h"
#include "Player/WS_PlayerController.h"
#include "WS_Utils.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"


void UWS_PlayerUIWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	HealthProgressBar->SetFillColorAndOpacity(HealthProgressBarColor);
	ManaProgressBar->SetFillColorAndOpacity(ManaProgressBarColor);
	StaminaProgressBar->SetFillColorAndOpacity(StaminaProgressBarColor);
}

float UWS_PlayerUIWidget::GetHealthPercent() const
{
	const auto HealthComponent = WS_Utils::GetPlayerComponent<UWS_HealthComponent>(GetOwningPlayerPawn());
	if (!HealthComponent)
	{
		return 0.0f;
	}
	return HealthComponent->GetHealthPercent();
}

float UWS_PlayerUIWidget::GetStaminaPercent() const
{
	const auto StaminaComponent = WS_Utils::GetPlayerComponent<UWS_StaminaComponent>(GetOwningPlayerPawn());
	if (!StaminaComponent)
	{
		return 0.0f;
	}
	return StaminaComponent->GetStaminaPercent();
}

float UWS_PlayerUIWidget::GetManaPercent() const
{
	const auto ManaComponent = WS_Utils::GetPlayerComponent<UWS_ManaComponent>(GetOwningPlayerPawn());
	if (!ManaComponent)
	{
		return 0.0f;
	}
	return ManaComponent->GetManaPercent();
}

bool UWS_PlayerUIWidget::IsPlayerAlive() const
{
	const auto HealthComponent = WS_Utils::GetPlayerComponent<UWS_HealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool UWS_PlayerUIWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool UWS_PlayerUIWidget::Initialize()
{
	const auto HealthComponent = WS_Utils::GetPlayerComponent<UWS_HealthComponent>(GetOwningPlayerPawn());
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &UWS_PlayerUIWidget::OnHealthChanged);
	}

	const auto StaminaComponent = WS_Utils::GetPlayerComponent<UWS_StaminaComponent>(GetOwningPlayerPawn());
	if (StaminaComponent)
	{
		StaminaComponent->OnStaminaChanged.AddUObject(this, &UWS_PlayerUIWidget::OnStaminaChanged);
	}

	const auto ManaComponent = WS_Utils::GetPlayerComponent<UWS_ManaComponent>(GetOwningPlayerPawn());
	if (ManaComponent)
	{
		ManaComponent->OnManaChanged.AddUObject(this, &UWS_PlayerUIWidget::OnManaChanged);
	}

	return Super::Initialize();
}

void UWS_PlayerUIWidget::SetHealthPercent(float HealthPercent)
{
	if (!HealthProgressBar)
	{
		return;
	}

	HealthProgressBar->SetPercent(HealthPercent);
}

void UWS_PlayerUIWidget::SetStaminaPercent(float StaminaPercent)
{
	if (!StaminaProgressBar)
	{
		return;
	}

	StaminaProgressBar->SetPercent(StaminaPercent);
}

void UWS_PlayerUIWidget::SetManaPercent(float ManaPercent)
{
	if (!ManaProgressBar)
	{
		return;
	}

	ManaProgressBar->SetPercent(ManaPercent);
}

void UWS_PlayerUIWidget::OnHealthChanged(float Health, float HealthDelta)
{
	if (HealthDelta < 0.0f)
	{
		OnTakeDamage();

		if (!IsAnimationPlaying(DamageAnimation))
		{
			PlayAnimation(DamageAnimation);
		}
	}

	SetHealthPercent(GetHealthPercent());
}

void UWS_PlayerUIWidget::OnStaminaChanged(float Stamina, float StaminaDelta)
{
	SetStaminaPercent(GetStaminaPercent());
}

void UWS_PlayerUIWidget::OnManaChanged(float Mana, float ManaDelta)
{
	SetManaPercent(GetManaPercent());
}