// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WS_WeaponComponent.generated.h"

class AWS_BaseWeapon;
class AWS_AuxiliaryWeapon;

DECLARE_MULTICAST_DELEGATE(FOnWantToStartAttackSignature);
DECLARE_MULTICAST_DELEGATE(FOnWantToStopAttackSignature);
DECLARE_MULTICAST_DELEGATE(FOnWantToStartDeflectSignature);
DECLARE_MULTICAST_DELEGATE(FOnWantToStopDeflectSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSuccessDeflectSignature, bool);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FG_WORKSAMPLE_API UWS_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UWS_WeaponComponent();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AWS_BaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (EditCondition = "bHasAuxiliaryWeapon"))
		TSubclassOf<AWS_AuxiliaryWeapon> AuxiliaryWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		bool bHasAuxiliaryWeapon = false;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName DefaultWeaponAttachPointName = "WeaponBack_Socket";

	//the name of the bone point of the mesh to attach a weapon, which is prepared for immediate use
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName WeaponAtTheReadyAttachPointName = "WeaponRight_Socket";

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		bool HasAuxiliaryWeapon() const { return bHasAuxiliaryWeapon; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void OnWantToStartAttacking();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void OnWantToStopAttacking();

	FOnWantToStartAttackSignature OnWantToStartAttack;
	FOnWantToStopAttackSignature OnWantToStopAttack;
	FOnWantToStartDeflectSignature OnWantToStartDeflect;
	FOnWantToStopDeflectSignature OnWantToStopDeflect;
	FOnSuccessDeflectSignature OnSuccessDeflect;

	void OnWantToStartDeflecting();
	void OnWantToStopDeflecting();
	void WeaponUse();
	void AuxiliaryWeaponUse();

private:

	UPROPERTY()
		AWS_BaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
		AWS_AuxiliaryWeapon* CurrentAuxiliaryWeapon = nullptr;

	void SpawnWeapon();
	void HasDeflectedTheBlow(bool IsSuccessfulDeflect);
		
};
