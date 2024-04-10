// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WS_MenuHUD.generated.h"

class UWS_MenuWidget;

UCLASS()
class FG_WORKSAMPLE_API AWS_MenuHUD : public AHUD
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UWS_MenuWidget> MenuWidgetClass;

	virtual void BeginPlay() override;
	
};
