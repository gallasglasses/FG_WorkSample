// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Components/WS_WeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

UWS_WeaponFXComponent::UWS_WeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UWS_WeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, Hit.ImpactPoint.Rotation());
}