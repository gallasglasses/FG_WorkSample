// Fill out your copyright notice in the Description page of Project Settings.


#include "WS_BasePickup.h"

#include "Player/WS_BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BasePickupLog, All, All);

AWS_BasePickup::AWS_BasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupStaticMesh");
	SetRootComponent(PickupStaticMesh);
	PickupStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	PickupStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void AWS_BasePickup::BeginPlay()
{
	Super::BeginPlay();

	if (bIsRotatingPickup)
	{
		GenerationRotationYaw();
	}
}

void AWS_BasePickup::TryTakePickup(AActor* OtherActor)
{
	UE_LOG(BasePickupLog, Display, TEXT("TryTakePickup"));
	const auto Pawn = Cast<AWS_BaseCharacter>(OtherActor);
	if (!Pawn) return;

	if (GivePickupTo(Pawn))
	{
		PickupWasTaken();
	}
}

//void AWS_BasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	Super::NotifyActorBeginOverlap(OtherActor);
//
//	const auto Pawn = Cast<AWS_BaseCharacter>(OtherActor);
//	if(!Pawn) return;
//
//	if (GivePickupTo(Pawn))
//	{
//		PickupWasTaken();
//	}
//}

void AWS_BasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsRotatingPickup)
	{
		AddActorLocalRotation(FRotator(0.f, RotationYaw, 0.f));
	}
}

bool AWS_BasePickup::GivePickupTo(AWS_BaseCharacter* PlayerPawn)
{
	return false;
}

void AWS_BasePickup::PickupWasTaken()
{
	UE_LOG(BasePickupLog, Display, TEXT("Pickup was destroy"));
	Destroy();
}

void AWS_BasePickup::GenerationRotationYaw()
{
	const auto Direction = FMath::RandBool() ? 1.f : -1.f;
	RotationYaw = FMath::RandRange(1.f, 2.f) * Direction;
}