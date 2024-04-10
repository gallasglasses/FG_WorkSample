// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "WS_AIPerceptionComponent.generated.h"

UCLASS()
class FG_WORKSAMPLE_API UWS_AIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:

	AActor* GetClosestEnemy() const;
	
};
