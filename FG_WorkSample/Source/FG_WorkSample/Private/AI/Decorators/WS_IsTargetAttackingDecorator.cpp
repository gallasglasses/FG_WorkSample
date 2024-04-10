// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorators/WS_IsTargetAttackingDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "WS_Utils.h"
#include "Components/WS_WeaponComponent.h"
#include "Interfaces/WS_ActionableFeedback.h"

UWS_IsTargetAttackingDecorator::UWS_IsTargetAttackingDecorator()
{
	NodeName = "Is Target Attacking";
}

bool UWS_IsTargetAttackingDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return false;

	const auto Enemy = Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName)->GetClass()->ImplementsInterface(UWS_ActionableFeedback::StaticClass());
	if (!Enemy) return false;

	return IWS_ActionableFeedback::Execute_IsAttackAction(Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
}

