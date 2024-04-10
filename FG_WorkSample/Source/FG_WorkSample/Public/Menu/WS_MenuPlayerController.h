// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WS_MenuPlayerController.generated.h"

UCLASS()
class FG_WORKSAMPLE_API AWS_MenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
};
