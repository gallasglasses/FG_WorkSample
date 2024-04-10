#pragma once

#include "Engine/DataTable.h"
#include "WS_CoreTypes.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameWidgetState);

UENUM(BlueprintType)
enum class EGameWidgetState : uint8
{
	WaitingToStart = 0,
	InProgress,
	Pause,
	PlayerMenu,
	Death,
	GameOver,
	TheEnd
};

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Neutral = 0,
	Friend = 1,
	Monster = 2
};

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	None = 0,
	Open,
	Loot
};

UENUM(BlueprintType)
enum class EGameLevel : uint8
{
	MenuLevel,
	LoadingLevel,
	StartLevel
};