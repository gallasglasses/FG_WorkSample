// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/WS_BaseCharacter.h"
#include "WS_AICharacter.generated.h"

class UAnimMontage;
class UBehaviorTree;
class UWidgetComponent;

class UWS_WeaponComponent;
class UWS_HealthComponent;
class AWS_HealthPickup;

DECLARE_MULTICAST_DELEGATE(FFinishedAttackSignature);

UCLASS()
class FG_WORKSAMPLE_API AWS_AICharacter : public ACharacter
{
	GENERATED_BODY()
	
public:

	AWS_AICharacter();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UWS_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UWS_WeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* DeflectAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		TArray<UAnimMontage*> AttackAnimMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamageVelocity = FVector2D(1000.f, 2000.f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamage = FVector2D(10.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		TSubclassOf<AWS_HealthPickup> HealthPickupClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float AttackRange = 0.f;

	virtual void BeginPlay() override;

	virtual void OnDeath(AActor* Killer);

public:

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsRunning() const { return bWantsToRun && !GetVelocity().IsZero(); };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsAttacking() const { return bIsAttacking;	};

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsAwakening() const { return bIsAwakening;	};

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsDeflecting() const { return bIsDeflecting; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool HasDeflectedTheBlow() const { return bHasDeflectedTheBlow; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsDamageDone() const { return bIsDamageDone; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool CanCounterattack() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		float GetAttackRange() const { return AttackRange; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetIsDamageDone(const bool IsDone) { bIsDamageDone = IsDone; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetIsAwakening(const bool IsDone) { bIsAwakening = IsDone; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetHasDeflectedTheBlow(const bool IsDone) { bHasDeflectedTheBlow = IsDone; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		float GetMovementDirection() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BehaviorTree")
		UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaTime) override;

	void OnStartAttacking();
	void OnStopAttacking();
	void OnStartDeflecting();
	void OnStopDeflecting();

	FFinishedAttackSignature FinishedAttack;

private:

	FTimerHandle AttackTimerHandle;
	FTimerHandle DeflectTimerHandle;
	FTimerHandle LifeStateTimerHandle;

	int8 AttackAnimIndex = 0;
	int8 RecoveryCount = 0;

	float TimeAnimMontage = 0.0f;

	bool bWantsToRun = false;
	bool bIsRunning = false;
	bool bIsAwakening = false;
	bool bIsMoving = false;
	bool bIsAttacking = false;
	bool bIsDamageDone = false;
	bool bIsDeflecting = false;
	bool bHasDeflectedTheBlow = false;
	//bool bIsInRange = false;

	void SuccessDeflected(bool IsSuccesDeflected);
	virtual void Attack();
	void Deflect();
	void ChooseAnimMontage();
	void OnHealthChanged(float Health, float HealthDelta);
	void OnPlayAnimMontage(int8 Count);
	void StopAttack();
	void AIDestroy();

	UFUNCTION()
		void OnGroundLanded(const FHitResult& Hit);
};
