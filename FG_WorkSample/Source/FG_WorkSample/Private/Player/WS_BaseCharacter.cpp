// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WS_BaseCharacter.h"
#include "Components/WS_CharacterMovementComponent.h"
#include "Components/WS_HealthComponent.h"
#include "Components/WS_StaminaComponent.h"
#include "Components/WS_ManaComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

AWS_BaseCharacter::AWS_BaseCharacter(const FObjectInitializer& ObjInit)
	:Super(ObjInit.SetDefaultSubobjectClass<UWS_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->TargetArmLength = 350.0f;
	SpringArmComponent->SocketOffset = FVector(-20.0f, 0.0f, 200.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetRelativeRotation(FRotator(0.0f, -20.0f, 0.0f));

	SwordTriggerHitComponent = CreateDefaultSubobject<UBoxComponent>("SwordTriggerHitComponent");
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
	SwordTriggerHitComponent->AttachToComponent(GetMesh(), AttachmentRules, "FX_Sword_Bottom");
	SwordTriggerHitComponent->OnComponentBeginOverlap.AddDynamic(this, &AWS_BaseCharacter::OnOverlapHit);
	SwordTriggerHitComponent->IgnoreActorWhenMoving(GetOwner(), true);

	HealthComponent = CreateDefaultSubobject<UWS_HealthComponent>("HealthComponent");
	StaminaComponent = CreateDefaultSubobject<UWS_StaminaComponent>("StaminaComponent");
	ManaComponent = CreateDefaultSubobject<UWS_ManaComponent>("ManaComponent");
}

void AWS_BaseCharacter::Move(const FInputActionValue& Value)
{
	//if () return; check movement state of character

	const FVector2D MovementVector = Value.Get<FVector2D>();

	bIsMoving = MovementVector.Y != 0 || MovementVector.X != 0;

	/*const FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, MovementVector.Y);

	const FVector Right = GetActorRightVector();
	AddMovementInput(Right, MovementVector.X);*/

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AWS_BaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AWS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(GetCharacterMovement());

	OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
	HealthComponent->OnDeath.AddUObject(this, &AWS_BaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AWS_BaseCharacter::OnHealthChanged);

	StaminaComponent->OnExhausted.AddUObject(this, &AWS_BaseCharacter::OnExhausted);
	//OnStaminaChanged(StaminaComponent->GetStamina(), 0.0f);

	//ManaComponent->OnMuggle.AddUObject(this, &AWS_BaseCharacter::OnMuggle);

	LandedDelegate.AddDynamic(this, &AWS_BaseCharacter::OnGroundLanded);

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BaseCharacterContext, 0);
		}
	}
	
}

void AWS_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWS_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AWS_BaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWS_BaseCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWS_BaseCharacter::Jump);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AWS_BaseCharacter::OnStartRunning);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AWS_BaseCharacter::OnStopRunning);
		/*EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AWS_BaseCharacter::Interact);*/
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AWS_BaseCharacter::Attack);
		EnhancedInputComponent->BindAction(DeflectAction, ETriggerEvent::Started, this, &AWS_BaseCharacter::Deflect);
	}
}

bool AWS_BaseCharacter::IsRunning() const
{
	return bIsRunning;
}

bool AWS_BaseCharacter::IsAttacking() const
{
	return bIsRunning;
}

bool AWS_BaseCharacter::IsDeflecting() const
{
	return bIsRunning;
}

bool AWS_BaseCharacter::IsMoving() const
{
	return bIsRunning;
}

float AWS_BaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;

	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

	return FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}

void AWS_BaseCharacter::Attack()
{
	if (bIsAttacking)
	{
		bIsComboAttack = true;

		UE_LOG(BaseCharacterLog, Display, TEXT("Attacking yet and save"));
	}
	else
	{
		bIsAttacking = true;
		ChooseComboAnimMontage();
		UE_LOG(BaseCharacterLog, Display, TEXT("Attacking now"));
	}
}

void AWS_BaseCharacter::Dead()
{
	OnDead.Broadcast();
	GetWorld()->GetTimerManager().ClearTimer(DeadTimerHandle);
}

void AWS_BaseCharacter::Deflect()
{
	if (bIsDeflecting)
	{
		UE_LOG(BaseCharacterLog, Display, TEXT("Deflecting yet"));
	}
	else
	{
		bIsDeflecting = true;
		UE_LOG(BaseCharacterLog, Display, TEXT("Deflect now"));
		PlayAnimMontage(DeflectAnimMontage);
		GetWorld()->GetTimerManager().SetTimer(DeflectTimerHandle, this, &AWS_BaseCharacter::OnStopDeflecting, DeflectAnimMontage->CalculateSequenceLength(), false);
		// need to do with not to take damage
	}
}

void AWS_BaseCharacter::ChooseComboAnimMontage()
{
	switch (ComboAttackCount)
	{
	case 0:
		OnPlayAnimMontage(0);
		ComboAttackCount = 1;
		RecoveryCount = 0;
		break;
	case 1:
		OnPlayAnimMontage(1);
		ComboAttackCount = 2;
		RecoveryCount = 1;
		break;
	case 2:
		OnPlayAnimMontage(2);
		ComboAttackCount = 3;
		RecoveryCount = 2;
		break;
	case 3:
		OnPlayAnimMontage(3);
		ComboAttackCount = 0;
		RecoveryCount = 3;
		break;
	default:
		break;
	}
}

void AWS_BaseCharacter::ComboAttackSave()
{
	if (bIsComboAttack)
	{
		bIsDamageDone = false;
		bIsComboAttack = false;
		ChooseComboAnimMontage();
	}
	else
	{
		ResetCombo();
	}
}

void AWS_BaseCharacter::ResetCombo()
{
	ComboAttackCount = 0;
	bIsComboAttack = false;
	OnStopAttacking();
	UE_LOG(BaseCharacterLog, Display, TEXT("Reset Combo"));
}

void AWS_BaseCharacter::Running()
{
	if (bWantsToRun && bIsMoving && !bIsRunning)
	{
		bIsRunning = true;
		OnGiveAnyStamina.Broadcast(true);
	}
	if (bIsMoving && !bWantsToRun)
	{
		bIsRunning = false;
	}
	if (!bIsMoving)
	{
		if (bWantsToRun)
		{
			bIsRunning = false;
			OnGiveAnyStamina.Broadcast(false);
		}
	}
}

void AWS_BaseCharacter::OnDeath()
{
	UE_LOG(BaseCharacterLog, Display, TEXT("%s, You are dead"), *GetName());
	PlayAnimMontage(DeathAnimMontage);
	GetCharacterMovement()->DisableMovement();
	bIsAttacking = false;
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetLifeSpan(DeathAnimMontage->CalculateSequenceLength() + 1.f); // TODO: Maybe not destroy the actor right away, do longer death scene

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, this, &AWS_BaseCharacter::Dead, DeathAnimMontage->CalculateSequenceLength(), false);

}

void AWS_BaseCharacter::OnExhausted()
{
	bWantsToRun = false;
}

void AWS_BaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
	/*HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));

	const auto HealthBarWidget = Cast<UWS_HealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if (!HealthBarWidget)
	{
		return;
	}
	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
	HealthBarWidget->SetScaleDamage(HealthDelta);*/
}

void AWS_BaseCharacter::OnPlayAnimMontage(int8 Count)
{
	if (bIsAttacking)
	{
		PlayAnimMontage(AttackAnimMontages[Count]);
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AWS_BaseCharacter::ComboAttackSave, AttackAnimMontages[Count]->CalculateSequenceLength(), false);
		UE_LOG(BaseCharacterLog, Display, TEXT("Play Attack %d"), Count);
	}
	else
	{
		PlayAnimMontage(RecoveryAnimMontages[Count]);
		UE_LOG(BaseCharacterLog, Display, TEXT("Play Recovery %d"), Count);
	}
}

void AWS_BaseCharacter::OnStartRunning()
{
	bWantsToRun = true;
	//UE_LOG(BaseCharacterLog, Display, TEXT("OnStartRunning"));

	Running();
}

void AWS_BaseCharacter::OnStopAttacking()
{
	bIsAttacking = false;
	bIsDamageDone = false;
	UE_LOG(BaseCharacterLog, Display, TEXT("Stop Attack"));
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	switch (RecoveryCount)
	{
	case 0:
		OnPlayAnimMontage(0);
		break;
	case 1:
		OnPlayAnimMontage(1);
		break;
	case 2:
		OnPlayAnimMontage(2);
		break;
	case 3:
		OnPlayAnimMontage(3);
		break;
	default:
		break;
	}
}

void AWS_BaseCharacter::OnStopRunning()
{
	bWantsToRun = false;
	bIsRunning = false;
	OnGiveAnyStamina.Broadcast(false);
	//UE_LOG(BaseCharacterLog, Display, TEXT("OnStopRunning"));
}

void AWS_BaseCharacter::OnStopDeflecting()
{
	bIsDeflecting = false;
	UE_LOG(BaseCharacterLog, Display, TEXT("Stop Deflect"));
	GetWorld()->GetTimerManager().ClearTimer(DeflectTimerHandle);
}

void AWS_BaseCharacter::MakeDamage(const FHitResult& HitResult)
{
	const auto HitActor = HitResult.GetActor();
	if(!HitActor) return;
	if (!bIsDamageDone)
	{
		HitActor->TakeDamage(WeaponDamageAmount, FDamageEvent(), GetPlayerController(), this);
		bIsDamageDone = true;
		//if (HitActor->Implements<UKillableObject>())
		//{
		//	//KillingComponent->ActorToKill = HitActor;
		//	KillingComponent->Kill(HitActor);
		//}
	}
}

APlayerController* AWS_BaseCharacter::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player)
	{
		return nullptr;
	}
	return Player->GetController<APlayerController>();
}

void AWS_BaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;

	UE_LOG(BaseCharacterLog, Display, TEXT("On Ground Landed %f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X) return;
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	UE_LOG(BaseCharacterLog, Display, TEXT("Final Damage %f"), FinalDamage);

	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void AWS_BaseCharacter::OnOverlapHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsAttacking())
	{
		const auto HitActor = SweepResult.GetActor();
		if (!HitActor)
		{
			return;
		}
		if (HitActor == this)
		{
			return;
		}
		UE_LOG(BaseCharacterLog, Display, TEXT("%s, you got damage"), *SweepResult.GetActor()->GetName());

		MakeDamage(SweepResult);
	}
}

