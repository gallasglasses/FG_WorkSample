// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WS_BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "AI/WS_AIController.h"
#include "Kismet/GameplayStatics.h"
#include "AI/WS_AICharacter.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

AWS_BaseWeapon::AWS_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
	WeaponMesh->IgnoreActorWhenMoving(GetOwner(), true);
}

void AWS_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
}

void AWS_BaseWeapon::OnStart()
{
}

void AWS_BaseWeapon::OnStop()
{
}

AController* AWS_BaseWeapon::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

