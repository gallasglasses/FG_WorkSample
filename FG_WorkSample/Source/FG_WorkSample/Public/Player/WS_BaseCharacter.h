// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WS_BaseCharacter.generated.h"

class UAnimMontage;
class UBoxComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;

class UWS_HealthComponent;
class UWS_ManaComponent;
class UWS_StaminaComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGiveAnyStaminaSignature, bool);
DECLARE_MULTICAST_DELEGATE(FOnGiveAnyManaSignature);
DECLARE_MULTICAST_DELEGATE(FDeadSignature);

UCLASS()
class FG_WORKSAMPLE_API AWS_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AWS_BaseCharacter(const FObjectInitializer& ObjInit);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* SwordTriggerHitComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UWS_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UWS_StaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UWS_ManaComponent* ManaComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputMappingContext* BaseCharacterContext;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* RunAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* DeflectAction;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* DeflectAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		TArray<UAnimMontage*> AttackAnimMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		TArray<UAnimMontage*> RecoveryAnimMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamageVelocity = FVector2D(1000.f, 2000.f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FVector2D LandedDamage = FVector2D(10.f, 100.f);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsAttacking() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsDeflecting() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsMoving() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		float GetMovementDirection() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float WeaponDamageAmount = 20.0f;

	FOnGiveAnyStaminaSignature OnGiveAnyStamina;
	FOnGiveAnyManaSignature OnGiveAnyMana;
	FDeadSignature OnDead;

private:

	FTimerHandle AttackTimerHandle;
	FTimerHandle DeflectTimerHandle;
	FTimerHandle DeadTimerHandle;

	int8 ComboAttackCount = 0;
	int8 RecoveryCount = 0;

	float TimeAnimMontage = 0.0f;

	bool bWantsToRun = false;
	bool bIsRunning = false;
	bool bIsMoving = false;
	bool bIsAttacking = false;
	bool bIsDamageDone = false;
	bool bIsComboAttack = false;
	bool bIsDeflecting = false;

	void Attack();
	void Dead();
	void Deflect();
	void ChooseComboAnimMontage();
	void ComboAttackSave();
	void ResetCombo();
	void Running();
	void OnDeath();
	void OnExhausted();
	void OnHealthChanged(float Health, float HealthDelta);
	void OnPlayAnimMontage(int8 Count);
	void OnStartRunning();
	void OnStopAttacking();
	void OnStopRunning();
	void OnStopDeflecting();
	void MakeDamage(const FHitResult& HitResult);

	APlayerController* GetPlayerController() const;

	UFUNCTION()
		void OnGroundLanded(const FHitResult& Hit);

	UFUNCTION()
		void OnOverlapHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
