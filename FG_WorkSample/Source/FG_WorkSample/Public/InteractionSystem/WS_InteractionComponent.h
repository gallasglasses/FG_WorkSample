// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "WS_CoreTypes.h"
#include "WS_InteractionComponent.generated.h"

class UWS_InteractWidget;

UCLASS(ClassGroup = "Collision", meta = (BlueprintSpawnableComponent, DisplayName = "Interaction Collision"))
class FG_WORKSAMPLE_API UWS_InteractionComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UWS_InteractionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UWS_InteractWidget> InteractMessageWidget;

	UPROPERTY()
		UWS_InteractWidget* InteractMessage;

	UPROPERTY()
		AActor* ActorToInteract;

	UFUNCTION()
		void OnInteractionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnInteractionCompEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void Interact();

	UFUNCTION()
		void StopInteract();

private:

	FTimerHandle HoldingTimerHandle;
	FTimerHandle UpdatePercentHoldingTimerHandle;

	float HoldingTime = 0.f;
	bool InProgress = false;
	EInteractionType ActorInteractionType = EInteractionType::None;

	void HideInteractMessage();
	void ShowInteractMessage();
	void PreExecuteInteract();
	void UpdatePercent();
};
