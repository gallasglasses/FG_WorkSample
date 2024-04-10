// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WS_BaseWeapon.h"
#include "WS_Sword.generated.h"

UCLASS()
class FG_WORKSAMPLE_API AWS_Sword : public AWS_BaseWeapon
{
	GENERATED_BODY()

public:

		AWS_Sword();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* WeaponTriggerHitComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnOverlapHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void MakeDamage(const FHitResult& HitResult);

public:

	virtual void OnStart() override;
	virtual void OnStop() override;
};
