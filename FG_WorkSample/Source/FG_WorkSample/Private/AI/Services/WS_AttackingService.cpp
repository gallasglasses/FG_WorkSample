// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/WS_AttackingService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "WS_Utils.h"
#include "Components/WS_WeaponComponent.h"

UWS_AttackingService::UWS_AttackingService()
{
	NodeName = "Attack";
}

void UWS_AttackingService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	
	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);
	if (Controller)
	{
		const auto WeaponComponent = WS_Utils::GetPlayerComponent<UWS_WeaponComponent>(Controller->GetPawn());
		if (WeaponComponent)
		{
			HasAim ? WeaponComponent->OnWantToStartAttacking() : WeaponComponent->OnWantToStopAttacking();
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
