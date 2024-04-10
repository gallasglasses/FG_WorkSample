// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WS_AuxiliaryWeapon.h"
#include "WS_Shield.generated.h"

class UBoxComponent;

UCLASS()
class FG_WORKSAMPLE_API AWS_Shield : public AWS_AuxiliaryWeapon
{
	GENERATED_BODY()

public:

	AWS_Shield();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Components")
		UBoxComponent* ShieldTriggerHitComponent;

	virtual void BeginPlay() override;

public:

	virtual void OnStart() override;
	virtual void OnStop() override;

private:

	UFUNCTION()
		void OnOverlapHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//void MakeDamage(const FHitResult& HitResult);

	AController* GetController() const;
};
