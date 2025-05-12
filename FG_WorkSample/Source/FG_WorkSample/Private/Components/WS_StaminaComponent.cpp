// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WS_StaminaComponent.h"
#include "Player/WS_BaseCharacter.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(StaminaComponentLog, All, All);

UWS_StaminaComponent::UWS_StaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWS_StaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxStamina > 0);

	SetStamina(MaxStamina);

	auto ComponentOwner = Cast<AWS_BaseCharacter>(GetOwner());
	if (ComponentOwner)
	{
		ComponentOwner->OnGiveAnyStamina.AddUObject(this, &UWS_StaminaComponent::OnGiveAnyStamina);
	}
}

void UWS_StaminaComponent::OnGiveAnyStamina(bool IsStaminaDischarge)
{
	if (!GetWorld()) return;
	auto ComponentOwner = Cast<AWS_BaseCharacter>(GetOwner());
	if (!ComponentOwner) return;

	if (IsStaminaDischarge && ComponentOwner->IsRunning())
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimerHandle);

		GetWorld()->GetTimerManager().SetTimer(StaminaDischargeTimerHandle, this, &UWS_StaminaComponent::StaminaDischargeUpdate, StaminaDischargeUpdateTime, true, StaminaDischargeDelay);
	}
	else if (!IsStaminaDischarge && !ComponentOwner->IsRunning())
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaDischargeTimerHandle);
		if (AutoStaminaRecovery)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRecoveryTimerHandle, this, &UWS_StaminaComponent::StaminaRecoveryUpdate, StaminaRecoveryUpdateTime, true, StaminaRecoveryDelay);
		}
	}
}

void UWS_StaminaComponent::StaminaRecoveryUpdate()
{
	SetStamina(Stamina + StaminaRecoveryModifier);

	if (FMath::IsNearlyEqual(Stamina, MaxStamina) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimerHandle);
	}
}

void UWS_StaminaComponent::StaminaDischargeUpdate()
{
	SetStamina(Stamina - StaminaDischargeModifier);
	if (IsExhausted() && GetWorld())
	{
		OnExhausted.Broadcast(); 
		GetWorld()->GetTimerManager().ClearTimer(StaminaDischargeTimerHandle);
	}
}

void UWS_StaminaComponent::SetStamina(float NewStamina)
{
	const auto NextStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
	const auto StaminaDelta = NextStamina - Stamina;

	UE_LOG(StaminaComponentLog, Display, TEXT("Delta %f Stamina %f NextStamina %f"), StaminaDelta, Stamina, NextStamina);
	Stamina = NextStamina;
	OnStaminaChanged.Broadcast(Stamina, StaminaDelta);
}