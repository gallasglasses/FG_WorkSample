// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WS_CoreTypes.h"
#include "WS_InteractableObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWS_InteractableObject : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractionFinished, AActor* /*InteractableObject*/, AActor* /*ActorInteractedWithObject*/);

class FG_WORKSAMPLE_API IWS_InteractableObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Interact(AActor* ActorInteractedWithObject);

	UFUNCTION()
		virtual void Interact_Implementation(AActor* ActorInteractedWithObject) {};

	UFUNCTION()
		virtual void NotifyInteractionFinished(AActor* InteractableObject, AActor* ActorInteractedWithObject);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		EInteractionType GetInteractionType();

	UFUNCTION()
		virtual EInteractionType GetInteractionType_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		float GetHoldingTimeAmount();

	UFUNCTION()
		virtual float GetHoldingTimeAmount_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetInteractionInfo(bool InProgress);

	UFUNCTION()
		virtual void SetInteractionInfo_Implementation(bool InProgress) {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool CanInteract();

	UFUNCTION()
		virtual bool CanInteract_Implementation() = 0;

	FOnInteractionFinished OnInteractionFinished;

};
