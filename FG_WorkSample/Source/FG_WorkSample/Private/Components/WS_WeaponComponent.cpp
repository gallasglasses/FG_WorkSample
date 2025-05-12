// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WS_WeaponComponent.h"
#include "Weapon/WS_BaseWeapon.h"
#include "Weapon/WS_AuxiliaryWeapon.h"
#include "GameFramework/Character.h"
#include "AI/WS_AICharacter.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

UWS_WeaponComponent::UWS_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWS_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}

void UWS_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon ->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon->Destroy();
	CurrentWeapon = nullptr;

	CurrentAuxiliaryWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentAuxiliaryWeapon->Destroy();
	CurrentAuxiliaryWeapon = nullptr;

	Super::EndPlay(EndPlayReason);
}

void UWS_WeaponComponent::OnWantToStartAttacking()
{
	if (CurrentWeapon)
	{
		OnWantToStartAttack.Broadcast();
		CurrentWeapon->OnStart();
	}
}

void UWS_WeaponComponent::OnWantToStopAttacking()
{
	if (CurrentWeapon)
	{
		OnWantToStopAttack.Broadcast();
		CurrentWeapon->OnStop();
	}
}

void UWS_WeaponComponent::OnWantToStartDeflecting()
{
	if (CurrentAuxiliaryWeapon && CurrentAuxiliaryWeapon->CanDeflect())
	{
		OnWantToStartDeflect.Broadcast();
		CurrentAuxiliaryWeapon->OnDeflectTheBlow.AddUObject(this, &UWS_WeaponComponent::HasDeflectedTheBlow);
	}
}

void UWS_WeaponComponent::OnWantToStopDeflecting()
{
	if (CurrentAuxiliaryWeapon && CurrentAuxiliaryWeapon->CanDeflect())
	{
		OnWantToStopDeflect.Broadcast();
	}
}

void UWS_WeaponComponent::WeaponUse()
{
	if (!CurrentWeapon) return;

	UE_LOG(WeaponComponentLog, Display, TEXT("WeaponUse"));
	CurrentWeapon->OnStart();
}

void UWS_WeaponComponent::AuxiliaryWeaponUse()
{
	if (!CurrentAuxiliaryWeapon) return;

	UE_LOG(WeaponComponentLog, Display, TEXT("AuxiliaryWeaponUse"));
	CurrentAuxiliaryWeapon->OnStart();
}

void UWS_WeaponComponent::SpawnWeapon()
{
	UE_LOG(WeaponComponentLog, Display, TEXT("Spawn Weapon now"));
	if (!GetWorld()) return;

	AWS_AICharacter* Character = Cast<AWS_AICharacter>(GetOwner());
	if (!Character) return;

	UE_LOG(WeaponComponentLog, Display, TEXT("Character"));

	CurrentWeapon = GetWorld()->SpawnActor<AWS_BaseWeapon>(WeaponClass);
	CurrentWeapon->SetOwner(GetOwner());
	if (!CurrentWeapon) return;

	UE_LOG(WeaponComponentLog, Display, TEXT("CurrentWeapon"));

	UE_LOG(WeaponComponentLog, Display, TEXT("Owner()->GetName() %s "), *GetOwner()->GetName());
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAtTheReadyAttachPointName);

	UE_LOG(WeaponComponentLog, Display, TEXT("AttachToComponent"));

	if (bHasAuxiliaryWeapon)
	{
		CurrentAuxiliaryWeapon = GetWorld()->SpawnActor<AWS_AuxiliaryWeapon>(AuxiliaryWeaponClass);
		CurrentAuxiliaryWeapon->SetOwner(GetOwner());
		if (!CurrentAuxiliaryWeapon) return;

		CurrentAuxiliaryWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, CurrentAuxiliaryWeapon->GetAuxiliaryWeaponAtTheReadyAttachPointName());

		UE_LOG(WeaponComponentLog, Display, TEXT("CurrentAuxiliaryWeapon"));
	}
}

void UWS_WeaponComponent::HasDeflectedTheBlow(bool IsSuccessfulDeflect)
{
	OnSuccessDeflect.Broadcast(IsSuccessfulDeflect);
}
