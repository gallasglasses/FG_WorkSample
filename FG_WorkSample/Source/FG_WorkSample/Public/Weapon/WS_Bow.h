// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WS_BaseWeapon.h"
#include "WS_Bow.generated.h"

class AWS_Projectile;

UCLASS()
class FG_WORKSAMPLE_API AWS_Bow : public AWS_BaseWeapon
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AWS_Projectile> ArrowClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName ArrowSocketName = "Arrow";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		float TraceMaxDistance = 1500.0f;

	FVector GetMuzzleWorldLocation() const;

	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	ACharacter* GetPlayer() const;

	void MakeShot();
	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

public:

	virtual void OnStart() override;
	virtual void OnStop() override;
};
