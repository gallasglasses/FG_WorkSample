// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/WS_AttackTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "WS_Utils.h"
#include "Components/WS_WeaponComponent.h"
#include "AI/WS_AICharacter.h"


UWS_AttackTask::UWS_AttackTask()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UWS_AttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto Owner = Cast<AWS_AICharacter>(OwnerComp.GetOwner());
	if (!Controller || !Blackboard || !Owner) return EBTNodeResult::Failed;
	
	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName); 
	const auto WeaponComponent = WS_Utils::GetPlayerComponent<UWS_WeaponComponent>(Controller->GetPawn());
	if (!WeaponComponent) return EBTNodeResult::Failed;

	HasAim ? WeaponComponent->OnWantToStartAttacking() : WeaponComponent->OnWantToStopAttacking();
	//Owner->FinishedAttack.AddUObject(this, &UWS_AttackTask::FinishedAttacking);
	return EBTNodeResult::Succeeded;
}
