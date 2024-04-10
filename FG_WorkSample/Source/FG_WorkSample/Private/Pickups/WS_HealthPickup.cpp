// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/WS_HealthPickup.h"
#include "WS_HealthPickup.h"
#include "Components/WS_HealthComponent.h"
#include "WS_Utils.h"
#include "Player/WS_BaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(HealthPickupLog, All, All);


void AWS_HealthPickup::OnInteractionFinished(AActor* ActorInteractedWithObject)
{
	NotifyInteractionFinished(this, ActorInteractedWithObject);
}

bool AWS_HealthPickup::GivePickupTo(AWS_BaseCharacter* PlayerPawn)
{
	const auto HealthComponent = WS_Utils::GetPlayerComponent<UWS_HealthComponent>(PlayerPawn);
	if(!HealthComponent) return false;

	UE_LOG(HealthPickupLog, Display, TEXT("Pickup here"));
	return HealthComponent->TryToAddHealth(HealthAmount);
}
