// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "WS_AIEnvQueryContext.generated.h"

UCLASS()
class FG_WORKSAMPLE_API UWS_AIEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

public:

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FName EnemyActorKeyName = "EnemyActor";
	
};
