// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/WS_AICharacter.h"
#include "WS_AIArcher.generated.h"

class UAnimMontage;

UCLASS()
class FG_WORKSAMPLE_API AWS_AIArcher : public AWS_AICharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* TakeArrowAnimMontage;

public:

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool HasEnemy() const { return bHasEnemy; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool HasArrow() const { return bHasArrow; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool IsReadyToShoot() const { return bIsReadyToShoot; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetHasEnemy(const bool IsEnemy) { bHasEnemy = IsEnemy; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetHasArrow(const bool IsArrow) { bHasArrow = IsArrow; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetIsReadyToShoot(const bool IsReady) { bIsReadyToShoot = IsReady; };

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void TakeArrow();

private:

	bool bHasEnemy = false;
	bool bHasArrow = false;
	bool bIsReadyToShoot = false;

};
