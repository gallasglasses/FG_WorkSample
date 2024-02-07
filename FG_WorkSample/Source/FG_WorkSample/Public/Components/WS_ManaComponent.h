// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WS_ManaComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMuggleSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnManaChangedSignature, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FG_WORKSAMPLE_API UWS_ManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UWS_ManaComponent();

	FOnMuggleSignature OnMuggle;
	FOnManaChangedSignature OnManaChanged;

	UFUNCTION(BlueprintCallable, Category = "Mana")
		bool IsMuggle() const { return FMath::IsNearlyZero(Mana); }

	UFUNCTION(BlueprintCallable, Category = "Mana")
		float GetManaPercent() const { return Mana / MaxMana; }

	float GetMana() const { return Mana; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mana", meta = (ClampMin = "0", ClampMax = "1000.0"))
		float MaxMana = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery")
		bool AutoManaRecovery = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery", meta = (EditCondition = "AutoHeal"))
		float ManaRecoveryUpdateTime = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery", meta = (EditCondition = "AutoHeal"))
		float ManaRecoveryDelay = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recovery", meta = (EditCondition = "AutoHeal"))
		float ManaRecoveryModifier = 5.f;

	virtual void BeginPlay() override;

private:

	FTimerHandle ManaRecoveryTimerHandle;
	float Mana = 0.0f;

	UFUNCTION()
		void OnGiveAnyMana();

	void ManaUpdate();
	void SetMana(float NewMana);
};
