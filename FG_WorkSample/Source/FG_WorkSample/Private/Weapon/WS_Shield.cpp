// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WS_Shield.h"
#include "Components/BoxComponent.h"
#include "AI/WS_AIController.h"
#include "AI/WS_AICharacter.h"
#include "Components/WS_HealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ShieldLog, All, All);

AWS_Shield::AWS_Shield()
{
	PrimaryActorTick.bCanEverTick = false;

	ShieldTriggerHitComponent = CreateDefaultSubobject<UBoxComponent>("ShieldTriggerHitComponent");
	ShieldTriggerHitComponent->SetupAttachment(AuxiliaryWeaponMesh);
	ShieldTriggerHitComponent->IgnoreActorWhenMoving(this->GetOwner(), true);
}

void AWS_Shield::BeginPlay()
{
	Super::BeginPlay();

	check(AuxiliaryWeaponMesh);
}

void AWS_Shield::OnStart()
{
	UE_LOG(ShieldLog, Display, TEXT("Owner: %s"), GetOwner());

	ShieldTriggerHitComponent->OnComponentBeginOverlap.AddDynamic(this, &AWS_Shield::OnOverlapHit);
}

void AWS_Shield::OnStop()
{
	ShieldTriggerHitComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AWS_Shield::OnOverlapHit);
}

void AWS_Shield::OnOverlapHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto HitActor = SweepResult.GetActor();
	if (!HitActor) return;

	AWS_AICharacter* Character = Cast<AWS_AICharacter>(GetOwner());
	if (!Character) return;
	
	if (Character->IsDeflecting())
	{
		OnDeflectTheBlow.Broadcast(true);
	}
}

void AWS_Shield::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWS_AICharacter* Character = Cast<AWS_AICharacter>(GetOwner());
	if (!Character) return;

	OnDeflectTheBlow.Broadcast(false);
}

AController* AWS_Shield::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
