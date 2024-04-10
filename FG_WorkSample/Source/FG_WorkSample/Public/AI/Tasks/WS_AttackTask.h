// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WS_AttackTask.generated.h"

/**
 * 
 */
UCLASS()
class FG_WORKSAMPLE_API UWS_AttackTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UWS_AttackTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector EnemyActorKey;

private:

	void FinishedAttacking();
};
