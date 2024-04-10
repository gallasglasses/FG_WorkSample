// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/WS_AICharacter.h"
#include "AI/WS_AIController.h"
#include "Components/WS_AIMovementComponent.h"
#include "Components/WS_HealthComponent.h"
#include "Components/WS_WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "BrainComponent.h"
#include "WS_HealthPickup.h"
#include "UI/WS_HealthBarWidget.h"


DEFINE_LOG_CATEGORY_STATIC(AICharacterLog, All, All);

AWS_AICharacter::AWS_AICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AWS_AIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
	}

	HealthComponent = CreateDefaultSubobject<UWS_HealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UWS_WeaponComponent>("WeaponComponent");

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->bOwnerNoSee = false;
	//HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HealthWidgetComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void AWS_AICharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(WeaponComponent);

	OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
	HealthComponent->OnDeathByInstigator.AddUObject(this, &AWS_AICharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AWS_AICharacter::OnHealthChanged);

	WeaponComponent->OnWantToStartAttack.AddUObject(this, &AWS_AICharacter::OnStartAttacking);
	WeaponComponent->OnWantToStopAttack.AddUObject(this, &AWS_AICharacter::OnStopAttacking);
	if (WeaponComponent->HasAuxiliaryWeapon())
	{
		WeaponComponent->OnWantToStartDeflect.AddUObject(this, &AWS_AICharacter::OnStartDeflecting);
		WeaponComponent->OnWantToStopDeflect.AddUObject(this, &AWS_AICharacter::OnStopDeflecting);
	}

	LandedDelegate.AddDynamic(this, &AWS_AICharacter::OnGroundLanded);

	//bIsAwakening = true;
}

void AWS_AICharacter::OnDeath(AActor* Killer)
{
	// AActor* Killer - add points for kill

	UE_LOG(AICharacterLog, Display, TEXT("%s, You are dead"), *GetName());
	const auto AIController = Cast<AWS_AIController>(Controller);
	if (AIController && AIController->BrainComponent)
	{
		UE_LOG(AICharacterLog, Display, TEXT("BrainComponent->Cleanup"));
		AIController->BrainComponent->Cleanup(); 
		OnStopAttacking();
		AIDestroy();
	}

	//GetCharacterMovement()->DisableMovement(); //UCharacterMovementComponent * GetCharacterMovement() const
	//GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//OnStopAttacking();
	//WeaponComponent->OnWantToStopAttacking();

	/*if (!Cast<AWS_BaseCharacter>(Killer))
	{
		AIDestroy();
	}*/
}

bool AWS_AICharacter::CanCounterattack() const
{
	return !HealthComponent->IsDead();
}

float AWS_AICharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero())
	{
		return 0;
	}
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void AWS_AICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWS_AICharacter::OnStartAttacking()
{
	UE_LOG(AICharacterLog, Display, TEXT("On Attack"));
	Attack();
}

void AWS_AICharacter::OnStopAttacking()
{
	//UE_LOG(AICharacterLog, Display, TEXT("On Stop Attack"));
	
	StopAttack();
}

void AWS_AICharacter::OnStartDeflecting()
{
	UE_LOG(AICharacterLog, Display, TEXT("On Deflect"));
	Deflect();
}

void AWS_AICharacter::OnHealthChanged(float Health, float HealthDelta)
{
	const auto HealthBarWidget = Cast<UWS_HealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if (!HealthBarWidget)
	{
		return;
	}
	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
	HealthBarWidget->SetScaleDamage(HealthDelta);
}

void AWS_AICharacter::OnPlayAnimMontage(int8 Count)
{
	if (bIsAttacking)
	{
		PlayAnimMontage(AttackAnimMontages[Count]);
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AWS_AICharacter::StopAttack, AttackAnimMontages[Count]->CalculateSequenceLength(), false);
		//UE_LOG(AICharacterLog, Display, TEXT("Play Attack %d"), Count);
	}
}

void AWS_AICharacter::OnStopDeflecting()
{
	bIsDeflecting = false;
	UE_LOG(AICharacterLog, Display, TEXT("Stop Deflect"));
	GetWorld()->GetTimerManager().ClearTimer(DeflectTimerHandle);
}

void AWS_AICharacter::StopAttack()
{
	FinishedAttack.Broadcast();
	bIsAttacking = false;
	bIsDamageDone = false;
	AttackAnimIndex = 0;
	//UE_LOG(AICharacterLog, Display, TEXT("bIsDamageDone %s"), bIsDamageDone ? TEXT("true") : TEXT("false"));
	UE_LOG(AICharacterLog, Display, TEXT("Stop Attack"));
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void AWS_AICharacter::AIDestroy()
{
	//UE_LOG(AICharacterLog, Display, TEXT("DeathAnimMontage"));
	PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	SetLifeSpan(DeathAnimMontage->CalculateSequenceLength() + 5.5f);

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetActorLocation());
	AWS_HealthPickup* HealthPickup = GetWorld()->SpawnActorDeferred<AWS_HealthPickup>(HealthPickupClass, SpawnTransform);
	if (HealthPickup)
	{
		HealthPickup->FinishSpawning(SpawnTransform);
	}

}

void AWS_AICharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;

	//UE_LOG(AICharacterLog, Display, TEXT("On Ground Landed %f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X) return;
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	//UE_LOG(AICharacterLog, Display, TEXT("Final Damage %f"), FinalDamage);

	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void AWS_AICharacter::SuccessDeflected(bool IsSuccesDeflected)
{
	//bHasDeflectedTheBlow = IsSuccesDeflected;
	HealthComponent->SetHasDeflectedDamage(IsSuccesDeflected);
}

void AWS_AICharacter::Attack()
{
	if (bIsAttacking)
	{
		UE_LOG(AICharacterLog, Display, TEXT("Attacking yet"));
	}
	else
	{
		bIsAttacking = true;
		UE_LOG(AICharacterLog, Display, TEXT("Attacking now"));
		
		ChooseAnimMontage();
	}
}

void AWS_AICharacter::Deflect()
{
	if (bIsDeflecting)
	{
		UE_LOG(AICharacterLog, Display, TEXT("Deflecting yet"));
	}
	else
	{
		bIsDeflecting = true;
		UE_LOG(AICharacterLog, Display, TEXT("Deflect now"));
		WeaponComponent->OnSuccessDeflect.AddUObject(this, &AWS_AICharacter::SuccessDeflected);
		PlayAnimMontage(DeflectAnimMontage);
		GetWorld()->GetTimerManager().SetTimer(DeflectTimerHandle, this, &AWS_AICharacter::OnStopDeflecting, DeflectAnimMontage->CalculateSequenceLength(), false);
		// need to do with not to take damage
	}
}

void AWS_AICharacter::ChooseAnimMontage()
{
	AttackAnimIndex = FMath::RandRange(0, AttackAnimMontages.Num()-1);

	UE_LOG(AICharacterLog, Display, TEXT("AttackAnimIndex : %i"), AttackAnimIndex);

	if (AttackAnimMontages.IsValidIndex(AttackAnimIndex))
	{
		OnPlayAnimMontage(AttackAnimIndex);
	}
}
