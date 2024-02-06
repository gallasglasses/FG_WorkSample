// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WS_BaseCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;

UCLASS()
class FG_WORKSAMPLE_API AWS_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AWS_BaseCharacter();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputMappingContext* BaseCharacterContext;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* DeflectAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void MoveForward(float Amount);
	void MoveRight(float Amount);

};
