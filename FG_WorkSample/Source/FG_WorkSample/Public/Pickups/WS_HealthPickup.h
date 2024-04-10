// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/WS_BasePickup.h"
#include "InteractionSystem/WS_InteractableObject.h"
#include "WS_CoreTypes.h"
#include "WS_HealthPickup.generated.h"

class AWS_AICharacter;

UCLASS()
class FG_WORKSAMPLE_API AWS_HealthPickup : public AWS_BasePickup, public IWS_InteractableObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
		float HealthAmount = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractableObject", meta = (ClampMin = "0.5", ClampMax = "100.0"))
		float HoldingTimeAmount = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractableObject")
		EInteractionType InteractionType = EInteractionType::Loot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractableObject")
		bool bCanInteract = true;


public:

	UFUNCTION(BlueprintCallable)
		void OnInteractionFinished(AActor* ActorInteractedWithObject);

	UFUNCTION()
		float GetHoldingTimeAmount_Implementation() override { return HoldingTimeAmount; };

	UFUNCTION()
		EInteractionType GetInteractionType_Implementation() override { return InteractionType; };

	UFUNCTION()
		bool CanInteract_Implementation() override { return bCanInteract; };

private:

	virtual bool GivePickupTo(AWS_BaseCharacter* PlayerPawn) override;
};
