// Fill out your copyright notice in the Description page of Project Settings.


#include "WS_GameInstance.h"
#include "UObject/SoftObjectPtr.h"

TSoftObjectPtr<UWorld> UWS_GameInstance::GetLevel(EGameLevel NewLevel) const
{
	if (!Levels.Find(NewLevel)) return nullptr;

	return Levels[NewLevel];
}

