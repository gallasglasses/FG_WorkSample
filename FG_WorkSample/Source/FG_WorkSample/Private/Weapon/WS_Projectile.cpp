// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WS_Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/WS_AIController.h"

DEFINE_LOG_CATEGORY_STATIC(ProjectileLog, All, All);

AWS_Projectile::AWS_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//CollisionComponent->bReturnMaterialOnMove = true;
	//CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	SetRootComponent(CollisionComponent);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = MoveSpeed;
	MovementComponent->ProjectileGravityScale = Mass;

}

void AWS_Projectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);
	check(CollisionComponent);

	StartPosition = GetActorLocation();

	MovementComponent->Velocity = GetActorForwardVector() * MovementComponent->InitialSpeed;
	MovementComponent->SetComponentTickEnabled(true);
	UE_LOG(ProjectileLog, Display, TEXT("Velocity: %s"), *MovementComponent->Velocity.ToString());

	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	UE_LOG(ProjectileLog, Display, TEXT("Owner: %s"), GetOwner());

	CollisionComponent->OnComponentHit.AddDynamic(this, &AWS_Projectile::OnProjectileHit);

	SetLifeSpan(LifeSeconds);
}

FVector AWS_Projectile::GetProjectileMovementDirection()
{
	const auto ShotDirectionNormal = ShotDirection.GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), ShotDirectionNormal));

	float Degrees = FMath::RadiansToDegrees(AngleBetween);
	FVector ProjectileDirection;
	return ProjectileDirection;
}

void AWS_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange)
	{
		Stop();
	}
}

void AWS_Projectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;

	Stop();
	const auto AIController = Cast<AWS_AIController>(GetController());
	if (AIController && AIController->GetTeamAttitudeTowards(*OtherActor) == ETeamAttitude::Hostile)
	{
		UE_LOG(ProjectileLog, Display, TEXT("Hostile"));

		UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, ShotDirection, Hit, GetController(), this, UDamageType::StaticClass());
	}
	else
	{
		UE_LOG(ProjectileLog, Display, TEXT("Friendly"));
	}
	Destroy();
}

void AWS_Projectile::Stop()
{
	MovementComponent->Velocity = FVector::ZeroVector;
	MovementComponent->SetComponentTickEnabled(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	ProjectileMesh->SetHiddenInGame(true);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

AController* AWS_Projectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
