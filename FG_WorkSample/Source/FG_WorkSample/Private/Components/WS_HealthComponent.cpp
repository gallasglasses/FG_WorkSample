// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WS_HealthComponent.h"
#include "Player/WS_BaseCharacter.h"
//#include "WS_Utils.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All);

UWS_HealthComponent::UWS_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

bool UWS_HealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull())
	{
		return false;
	}
	SetHealth(Health + HealthAmount);
	return true;
}

bool UWS_HealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void UWS_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UWS_HealthComponent::OnTakeAnyDamage);
	}
}

void UWS_HealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f || IsDead() || !GetWorld() || !DamagedActor) return;

	const auto CharacterDamaged = Cast<AWS_BaseCharacter>(DamagedActor);
	/*if (CharacterDamaged && CharacterDamaged->IsBlocking())
	{
		const auto AbilitiesComponent = WS_Utils::GetPlayerComponent<UKP_CharacterAbilitiesComponent>(CharacterDamaged);
		if (!AbilitiesComponent) return;

		const auto Shield = Cast<AKP_Shield>(AbilitiesComponent->GetShield());
		if (!Shield) return;
		Damage = Damage * Shield->GetTransmittedDamagePercent();
	}*/

	UE_LOG(HealthComponentLog, Display, TEXT("Damage: %f"), Damage);
	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead() && CharacterDamaged)
	{
		OnDeath.Broadcast();
	}
	else if (IsDead())
	{
		OnDeathByInstigator.Broadcast(DamagedActor);
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UWS_HealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}

void UWS_HealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UWS_HealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	UE_LOG(HealthComponentLog, Display, TEXT("Delta %f Health %f NextHealth %f"), HealthDelta, Health, NextHealth);
	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta);
}

