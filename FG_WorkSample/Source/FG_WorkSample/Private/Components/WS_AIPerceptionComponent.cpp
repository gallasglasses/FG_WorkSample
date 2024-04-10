// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WS_AIPerceptionComponent.h"
#include "AIController.h"
#include "WS_Utils.h"
#include "Components/WS_HealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

AActor* UWS_AIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PercieveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
	if(PercieveActors.Num() == 0) 
	{
		GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);
		if (PercieveActors.Num() == 0) return nullptr;
	}

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;

	for (const auto PercieveActor : PercieveActors)
	{
		if (!PercieveActor) return nullptr;
		const auto BehaviorType = Controller->GetTeamAttitudeTowards(*PercieveActor);

		const auto HealthComponent = WS_Utils::GetPlayerComponent<UWS_HealthComponent>(PercieveActor);
		if (BehaviorType == ETeamAttitude::Hostile && HealthComponent && !HealthComponent->IsDead())
		{
			const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PercieveActor;
			}
		}
	}

	return BestPawn;
}
