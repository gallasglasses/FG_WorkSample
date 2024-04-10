// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WS_BasePickup.generated.h"

class AWS_BaseCharacter;
class USphereComponent;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class FG_WORKSAMPLE_API AWS_BasePickup : public AActor
{
	GENERATED_BODY()

public:

	AWS_BasePickup();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
		USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
		UStaticMeshComponent* PickupStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		bool bIsRotatingPickup = false;

	virtual void BeginPlay() override;
	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void TryTakePickup(AActor* OtherActor);

public:

	virtual void Tick(float DeltaTime) override;

private:

	float RotationYaw = 0.f;

	void PickupWasTaken();
	void GenerationRotationYaw();
	virtual bool GivePickupTo(AWS_BaseCharacter* PlayerPawn);
};

