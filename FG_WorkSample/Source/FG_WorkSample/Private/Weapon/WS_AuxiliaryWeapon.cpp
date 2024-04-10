// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WS_AuxiliaryWeapon.h"
#include "Components/SkeletalMeshComponent.h "

AWS_AuxiliaryWeapon::AWS_AuxiliaryWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	AuxiliaryWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("AuxiliaryWeaponMesh");
	SetRootComponent(AuxiliaryWeaponMesh);
	AuxiliaryWeaponMesh->IgnoreActorWhenMoving(GetOwner(), true);
}

void AWS_AuxiliaryWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(AuxiliaryWeaponMesh);
}

