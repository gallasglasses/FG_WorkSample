// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WS_Bow.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Weapon/WS_Projectile.h"

DEFINE_LOG_CATEGORY_STATIC(BowLog, All, All);

FVector AWS_Bow::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(ArrowSocketName);
}

void AWS_Bow::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld())
        return;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
    UE_LOG(BowLog, Display, TEXT("MakeHit"));
}

ACharacter* AWS_Bow::GetPlayer() const
{
    return !Cast<ACharacter>(GetOwner()) ? nullptr : Cast<ACharacter>(GetOwner());
}

void AWS_Bow::OnStart()
{
    UE_LOG(BowLog, Display, TEXT("Throw Arrow!"));

    MakeShot();
}

void AWS_Bow::OnStop()
{
}

void AWS_Bow::MakeShot()
{
    if (!GetWorld())
        return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        UE_LOG(BowLog, Display, TEXT("GetTraceData FALSE"));
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint + GetMuzzleWorldLocation().GetSafeNormal());

    //DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Magenta, false, 1.f, 0, 3.f);
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    AWS_Projectile* Projectile = GetWorld()->SpawnActorDeferred<AWS_Projectile>(ArrowClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetActorRotator(GetPlayer()->GetActorRotation());
        Projectile->SetDamageAmount(DamageAmount);
        UE_LOG(BowLog, Display, TEXT("Direction: %s"), *Direction.ToString());

        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }
    else
    {
        UE_LOG(BowLog, Display, TEXT("Projectile NULL"));
        return;
    }
}

bool AWS_Bow::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    UE_LOG(BowLog, Display, TEXT("GetTraceData"));
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        UE_LOG(BowLog, Display, TEXT("GetPlayerViewPoint FALSE"));
        return false;
    }

    TraceStart = ViewLocation;

    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    UE_LOG(BowLog, Display, TEXT("TraceEnd %s"), *TraceEnd.ToString());

    return true;
}

bool AWS_Bow::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Character) return false;

    ViewLocation = GetMuzzleWorldLocation();
    ViewRotation = WeaponMesh->GetSocketRotation(ArrowSocketName);

    return true;
}
