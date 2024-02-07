// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WS_StaminaComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnExhaustedSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedSignature, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FG_WORKSAMPLE_API UWS_StaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UWS_StaminaComponent();

	FOnExhaustedSignature OnExhausted;
	FOnStaminaChangedSignature OnStaminaChanged;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
		bool IsExhausted() const { return FMath::IsNearlyZero(Stamina); }

	UFUNCTION(BlueprintCallable, Category = "Stamina")
		float GetStaminaPercent() const { return Stamina / MaxStamina; }

	float GetStamina() const { return Stamina; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina", meta = (ClampMin = "0", ClampMax = "1000.0"))
		float MaxStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery")
		bool AutoStaminaRecovery = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery", meta = (EditCondition = "AutoHeal"))
		float StaminaRecoveryUpdateTime = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery", meta = (EditCondition = "AutoHeal"))
		float StaminaRecoveryDelay = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery", meta = (EditCondition = "AutoHeal"))
		float StaminaRecoveryModifier = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery", meta = (EditCondition = "Discharge"))
		float StaminaDischargeUpdateTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery", meta = (EditCondition = "Discharge"))
		float StaminaDischargeDelay = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery", meta = (EditCondition = "Discharge"))
		float StaminaDischargeModifier = 0.5f;

	virtual void BeginPlay() override;

private:

	FTimerHandle StaminaDischargeTimerHandle;
	FTimerHandle StaminaRecoveryTimerHandle;

	float Stamina = 0.0f;

	UFUNCTION()
		void OnGiveAnyStamina(bool IsStaminaDischarge);

	void StaminaRecoveryUpdate();
	void StaminaDischargeUpdate();
	void SetStamina(float NewStamina);
		
};
