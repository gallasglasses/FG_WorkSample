// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WS_BaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class FG_WORKSAMPLE_API AWS_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:

		AWS_BaseWeapon();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float DamageAmount = 10.0f;

	AController* GetController() const;

	virtual void BeginPlay() override;

public:

	virtual void OnStart();
	virtual void OnStop();

};
