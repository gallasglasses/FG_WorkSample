// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WS_Sword.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/WS_AIController.h"
#include "Kismet/GameplayStatics.h"
#include "AI/WS_AICharacter.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(SwordLog, All, All);

AWS_Sword::AWS_Sword()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponTriggerHitComponent = CreateDefaultSubobject<UBoxComponent>("WeaponTriggerHitComponent");
	WeaponTriggerHitComponent->SetupAttachment(WeaponMesh);
	WeaponTriggerHitComponent->IgnoreActorWhenMoving(this->GetOwner(), true);

}

void AWS_Sword::BeginPlay()
{
	Super::BeginPlay();

	WeaponTriggerHitComponent->OnComponentBeginOverlap.AddDynamic(this, &AWS_Sword::OnOverlapHit);
}

void AWS_Sword::OnStart()
{
	UE_LOG(SwordLog, Display, TEXT("Owner: %s"), *GetOwner()->GetName());
	UE_LOG(SwordLog, Display, TEXT("OnStart()"));

}

void AWS_Sword::OnStop()
{
	UE_LOG(SwordLog, Display, TEXT("OnStop()"));
}

void AWS_Sword::OnOverlapHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto HitActor = SweepResult.GetActor();
	if (!HitActor) return;

	const auto AICharacter = Cast<AWS_AICharacter>(GetOwner());
	if (!AICharacter || HitActor == AICharacter || HitActor == this || HitActor->GetOwner() == AICharacter) return;

	if (AICharacter->IsAttacking())
	{
		const auto AIController = Cast<AWS_AIController>(GetController());
		if (AIController  && AIController->GetTeamAttitudeTowards(*OtherActor) == ETeamAttitude::Hostile && Cast<UCapsuleComponent>(SweepResult.GetComponent()))
		{
			UE_LOG(SwordLog, Display, TEXT("Hostile: %s, you got damage"), *SweepResult.GetActor()->GetName());
			UE_LOG(SwordLog, Display, TEXT("GetComponent: %s"), *SweepResult.GetComponent()->GetName());
			MakeDamage(SweepResult);
		}
		else
		{
			UE_LOG(SwordLog, Display, TEXT("Friendly"));
		}
	}
}

void AWS_Sword::MakeDamage(const FHitResult& HitResult)
{
	const auto HitActor = HitResult.GetActor();

	const auto AICharacter = Cast<AWS_AICharacter>(GetOwner());
	if (!AICharacter || HitActor == AICharacter) return;

	UE_LOG(SwordLog, Display, TEXT("bIsDamageDone %s"), AICharacter->IsDamageDone() ? TEXT("true") : TEXT("false"));
	
	if (!AICharacter->IsDamageDone())
	{
		UE_LOG(SwordLog, Display, TEXT("Damage done"));
		HitActor->TakeDamage(DamageAmount, FDamageEvent{}, GetController(), this);
		AICharacter->SetIsDamageDone(true);
	}
}
