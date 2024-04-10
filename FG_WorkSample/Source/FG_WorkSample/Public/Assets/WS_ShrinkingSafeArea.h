// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WS_ShrinkingSafeArea.generated.h"

class UStaticMeshComponent;
class AWS_BaseCharacter;

UCLASS()
class FG_WORKSAMPLE_API AWS_ShrinkingSafeArea : public AActor
{
	GENERATED_BODY()
	
public:	

	AWS_ShrinkingSafeArea();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SafeArea")
		UStaticMeshComponent* SafeAreaStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SafeArea", meta = (ClampMin = "0.5", ClampMax = "100.0"))
		float DamageAmount = 1.f;

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

public:	

	virtual void Tick(float DeltaTime) override;

private:

	FTimerHandle DamageTimerHandle;
	AWS_BaseCharacter* BaseCharacter = nullptr;

	void MakeDamage();
	void OnDead();
	
};
