// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WS_ActionableFeedback.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWS_ActionableFeedback : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FG_WORKSAMPLE_API IWS_ActionableFeedback
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool IsAttackAction();

	/*UFUNCTION()
		virtual bool IsAttackAction_Implementation() {};*/

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool IsDeflectAction();

	/*UFUNCTION()
		virtual bool IsDeflectAction_Implementation() {};*/
};
