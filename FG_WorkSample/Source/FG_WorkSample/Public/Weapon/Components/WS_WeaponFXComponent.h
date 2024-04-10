// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WS_WeaponFXComponent.generated.h"

class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FG_WORKSAMPLE_API UWS_WeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UWS_WeaponFXComponent();

	void PlayImpactFX(const FHitResult& Hit);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		UNiagaraSystem* Effect;
};
