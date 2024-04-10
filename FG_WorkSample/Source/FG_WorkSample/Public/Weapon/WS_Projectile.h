// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WS_Projectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class FG_WORKSAMPLE_API AWS_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AWS_Projectile();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Fire Parametrs")
		float LifeSeconds = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Fire Parametrs")
		float MoveSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Fire Parametrs")
		float FireRange = 3000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Fire Parametrs")
		float Mass = 10.0f;


public:

	virtual void Tick(float DeltaTime) override;
	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }
	void SetStartPosition(const FVector& Position) { StartPosition = Position; }
	void SetActorRotator(const FRotator& Rotator) { ActorRotator = Rotator; }
	void SetDamageAmount(const float Damage) { DamageAmount = Damage; }

private:

	FVector ShotDirection;
	FVector StartPosition;
	FRotator ActorRotator;
	float DamageAmount;

	UFUNCTION()
		void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Stop();
	AController* GetController() const;
	FVector GetProjectileMovementDirection();
};
