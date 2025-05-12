// Fill out your copyright notice in the Description page of Project Settings.


#include "Assets/WS_ShrinkingSafeArea.h"
#include "Components/StaticMeshComponent.h"
#include "Player/WS_BaseCharacter.h"
#include "WS_Utils.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(ShrinkingSafeAreaLog, All, All);

AWS_ShrinkingSafeArea::AWS_ShrinkingSafeArea()
{
	PrimaryActorTick.bCanEverTick = true;

	SafeAreaStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("SafeArea");
	SetRootComponent(SafeAreaStaticMesh);
	SafeAreaStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SafeAreaStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SafeAreaStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWS_ShrinkingSafeArea::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		BaseCharacter = Cast<AWS_BaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (BaseCharacter)
		{
			UE_LOG(ShrinkingSafeAreaLog, Display, TEXT("OnDead.AddUObject this, &AWS_ShrinkingSafeArea::OnDead"));
			BaseCharacter->OnDead.AddUObject(this, &AWS_ShrinkingSafeArea::OnDead);
		}
	}
}

void AWS_ShrinkingSafeArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWS_ShrinkingSafeArea::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Cast<AWS_BaseCharacter>(OtherActor))
	{
		BaseCharacter = Cast<AWS_BaseCharacter>(OtherActor);

		UE_LOG(ShrinkingSafeAreaLog, Display, TEXT("OtherActor : %s"), *OtherActor->GetName());
		UE_LOG(ShrinkingSafeAreaLog, Display, TEXT("BaseCharacter : %s"), *BaseCharacter->GetName());

		BaseCharacter->SetIsInsideSafeZone(true);
		BaseCharacter->ChangePostProcess(false);

		UE_LOG(ShrinkingSafeAreaLog, Display, TEXT("Inside ShrinkingSafeAreaLog"));

		if (GetWorld()->GetTimerManager().IsTimerActive(DamageTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
		}
	}
	
}

void AWS_ShrinkingSafeArea::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (Cast<AWS_BaseCharacter>(OtherActor))
	{
		BaseCharacter = Cast<AWS_BaseCharacter>(OtherActor);

		UE_LOG(ShrinkingSafeAreaLog, Display, TEXT("OtherActor : %s"), *OtherActor->GetName());
		UE_LOG(ShrinkingSafeAreaLog, Display, TEXT("BaseCharacter : %s"), *BaseCharacter->GetName());

		BaseCharacter->SetIsInsideSafeZone(false);
		BaseCharacter->ChangePostProcess(true);

		UE_LOG(ShrinkingSafeAreaLog, Display, TEXT("Outside ShrinkingSafeAreaLog"));

		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &AWS_ShrinkingSafeArea::MakeDamage, 1.f, true);
	}
}

void AWS_ShrinkingSafeArea::MakeDamage()
{
	BaseCharacter->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
}

void AWS_ShrinkingSafeArea::OnDead()
{
	UE_LOG(ShrinkingSafeAreaLog, Display, TEXT("OnDead"));

	if (GetWorld()->GetTimerManager().IsTimerActive(DamageTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
	}
}
