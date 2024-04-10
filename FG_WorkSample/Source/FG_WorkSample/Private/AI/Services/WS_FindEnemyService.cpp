// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/WS_FindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "WS_Utils.h"
#include "Components/WS_AIPerceptionComponent.h"

UWS_FindEnemyService::UWS_FindEnemyService()
{
	NodeName = "Find Enemy";
}

void UWS_FindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = WS_Utils::GetPlayerComponent<UWS_AIPerceptionComponent>(Controller);
		if (PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
		}
		Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	}
}
