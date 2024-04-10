// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "WS_AttackingService.generated.h"

UCLASS()
class FG_WORKSAMPLE_API UWS_AttackingService : public UBTService
{
	GENERATED_BODY()

public:

	UWS_AttackingService();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
