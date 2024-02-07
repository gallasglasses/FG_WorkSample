// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WS_CharacterMovementComponent.h"
#include "Player/WS_BaseCharacter.h"

float UWS_CharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const AWS_BaseCharacter* Player = Cast<AWS_BaseCharacter>(GetPawnOwner());
	
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}