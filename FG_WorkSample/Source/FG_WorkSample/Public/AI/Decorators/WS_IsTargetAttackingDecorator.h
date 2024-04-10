// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "WS_IsTargetAttackingDecorator.generated.h"

/**
 * 
 */
UCLASS()
class FG_WORKSAMPLE_API UWS_IsTargetAttackingDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:

	UWS_IsTargetAttackingDecorator();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector EnemyActorKey;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
