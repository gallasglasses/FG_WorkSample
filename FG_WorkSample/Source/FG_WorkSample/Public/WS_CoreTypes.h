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
	QuestList,
	Death,
	GameOver
};