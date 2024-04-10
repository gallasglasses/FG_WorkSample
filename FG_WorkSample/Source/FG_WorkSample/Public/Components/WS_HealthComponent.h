// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WS_HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeathByInstigatorSignature, AActor*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);
DECLARE_MULTICAST_DELEGATE(FOnStopInteractionSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FG_WORKSAMPLE_API UWS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UWS_HealthComponent();

	FOnDeathSignature OnDeath;
	FOnDeathByInstigatorSignature OnDeathByInstigator;
	FOnHealthChangedSignature OnHealthChanged;
	FOnStopInteractionSignature OnStopInteraction;

	UFUNCTION(BlueprintCallable, Category = "Health")
		bool IsDead() const { return FMath::IsNearlyZero(Health); }

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealthPercent() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetHasDeflectedDamage(const bool IsDone) { bHasDeflectedDamage = IsDone; };

	float GetHealth() const { return Health; }

	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0", ClampMax = "1000.0"))
		float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
		bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealUpdateTime = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealDelay = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
		float HealModifier = 5.f;
	
	virtual void BeginPlay() override;

private:

	FTimerHandle HealTimerHandle;

	float Health = 0.0f;

	bool bHasDeflectedDamage = false;

	UFUNCTION()
		void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();
	void SetHealth(float NewHealth);

	void ReportDamage(float Damage, AController* InstigatedBy);
};
