// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WS_AuxiliaryWeapon.generated.h"

class USkeletalMeshComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeflectTheBlowSignature, bool);

UCLASS()
class FG_WORKSAMPLE_API AWS_AuxiliaryWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AWS_AuxiliaryWeapon();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		USkeletalMeshComponent* AuxiliaryWeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName DefaultAuxiliaryWeaponAttachPointName = "ShieldBack_Socket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName AuxiliaryWeaponAtTheReadyAttachPointName = "Shield_Socket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		bool bCanDeflect = false;

	virtual void BeginPlay() override;

public:

	FOnDeflectTheBlowSignature OnDeflectTheBlow;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		FName GetAuxiliaryWeaponAtTheReadyAttachPointName() const { return AuxiliaryWeaponAtTheReadyAttachPointName; };

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		bool CanDeflect() const { return bCanDeflect; }

	virtual void OnStart() {};
	virtual void OnStop() {};

};
