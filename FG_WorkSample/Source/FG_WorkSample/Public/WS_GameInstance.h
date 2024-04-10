// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WS_CoreTypes.h"
#include "WS_GameInstance.generated.h"


UCLASS()
class FG_WORKSAMPLE_API UWS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	FName GetStartLevelName() const { return StartLevelName; };
	FName GetMenuLevelName() const { return MenuLevelName; };
	TSoftObjectPtr<UWorld> GetLevel(EGameLevel NewLevel) const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Levels")
		FName StartLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "Levels")
		FName MenuLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Transient, BlueprintReadOnly, Category = "Levels")
		TMap<EGameLevel, TSoftObjectPtr<UWorld>> Levels;
};
