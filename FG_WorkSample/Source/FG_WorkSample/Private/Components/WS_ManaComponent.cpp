// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WS_ManaComponent.h"
#include "Player/WS_BaseCharacter.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(ManaComponentLog, All, All);

UWS_ManaComponent::UWS_ManaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWS_ManaComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxMana > 0);

	SetMana(MaxMana);

	auto ComponentOwner = Cast<AWS_BaseCharacter>(GetOwner());
	if (ComponentOwner)
	{
		ComponentOwner->OnGiveAnyMana.AddUObject(this, &UWS_ManaComponent::OnGiveAnyMana);
	}

}

void UWS_ManaComponent::OnGiveAnyMana()
{
	if (!GetWorld()) return;

	SetMana(Mana - 10.f);

	GetWorld()->GetTimerManager().ClearTimer(ManaRecoveryTimerHandle);

	if (IsMuggle())
	{
		OnMuggle.Broadcast();
	}
	if (AutoManaRecovery)
	{
		GetWorld()->GetTimerManager().SetTimer(ManaRecoveryTimerHandle, this, &UWS_ManaComponent::ManaUpdate, ManaRecoveryUpdateTime, true, ManaRecoveryDelay);
	}
}

void UWS_ManaComponent::ManaUpdate()
{
	SetMana(Mana + ManaRecoveryModifier);

	if (FMath::IsNearlyEqual(Mana, MaxMana) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ManaRecoveryTimerHandle);
	}
}

void UWS_ManaComponent::SetMana(float NewMana)
{
	const auto NextMana = FMath::Clamp(NewMana, 0.0f, MaxMana);
	const auto ManaDelta = NextMana - Mana;

	UE_LOG(ManaComponentLog, Display, TEXT("Delta %f Mana %f NextMana %f"), ManaDelta, Mana, NextMana);
	Mana = NextMana;
	OnManaChanged.Broadcast(Mana, ManaDelta);
}