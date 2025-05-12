// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/WS_InteractionComponent.h"
#include "InteractionSystem/WS_InteractableObject.h"
#include "UI/WS_InteractWidget.h"
#include "Blueprint/UserWidget.h"
#include "Player/WS_BaseCharacter.h"
#include "Components/WS_HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WS_Utils.h"

DEFINE_LOG_CATEGORY_STATIC(InteractionComponentLog, All, All);

UWS_InteractionComponent::UWS_InteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWS_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractionCompBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractionCompEndOverlap);

	const auto ComponentOwner = Cast<AWS_BaseCharacter>(GetOwner());
	if (ComponentOwner)
	{
		ComponentOwner->OnStopInteraction.AddUObject(this, &UWS_InteractionComponent::StopInteract);
	}
	const auto HealthComponent = WS_Utils::GetPlayerComponent<UWS_HealthComponent>(ComponentOwner);
	if (HealthComponent)
	{
		HealthComponent->OnStopInteraction.AddUObject(this, &UWS_InteractionComponent::StopInteract);
	}
}

void UWS_InteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWS_InteractionComponent::Interact()
{
	if (ActorToInteract && InteractMessage && IWS_InteractableObject::Execute_CanInteract(ActorToInteract))
	{
		ActorInteractionType = IWS_InteractableObject::Execute_GetInteractionType(ActorToInteract);
		if (ActorInteractionType == EInteractionType::Loot)
		{
			UE_LOG(InteractionComponentLog, Display, TEXT("Interact | Loot"));
			
		}
		else if (ActorInteractionType == EInteractionType::Open)
		{
			UE_LOG(InteractionComponentLog, Display, TEXT("Interact | Open"));
			IWS_InteractableObject::Execute_SetInteractionInfo(ActorToInteract, true);
			InProgress = true;
		}

		HoldingTime = IWS_InteractableObject::Execute_GetHoldingTimeAmount(ActorToInteract);
		InteractMessage->SetMaxHoldingTime(HoldingTime);
		GetWorld()->GetTimerManager().SetTimer(HoldingTimerHandle, this, &UWS_InteractionComponent::PreExecuteInteract, HoldingTime, false);
		GetWorld()->GetTimerManager().SetTimer(UpdatePercentHoldingTimerHandle, this, &UWS_InteractionComponent::UpdatePercent, 0.1f, true);
	}
}

void UWS_InteractionComponent::StopInteract()
{
	if (ActorToInteract && IWS_InteractableObject::Execute_CanInteract(ActorToInteract))
	{
		if (ActorInteractionType == EInteractionType::Open)
		{
			UE_LOG(InteractionComponentLog, Display, TEXT("StopInteract | Open"));
			InProgress = false;
			IWS_InteractableObject::Execute_SetInteractionInfo(ActorToInteract, false);
			const auto Time = HoldingTime - GetWorld()->GetTimerManager().GetTimerRemaining(HoldingTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(HoldingTimerHandle, this, &UWS_InteractionComponent::ShowInteractMessage, Time, false);
			HideInteractMessage();
		}
		else
		{
			UE_LOG(InteractionComponentLog, Display, TEXT("StopInteract"));
			GetWorld()->GetTimerManager().ClearTimer(HoldingTimerHandle);
			GetWorld()->GetTimerManager().ClearTimer(UpdatePercentHoldingTimerHandle);
			HideInteractMessage();
			ShowInteractMessage();
		}
	}
}

void UWS_InteractionComponent::OnInteractionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UWS_InteractableObject>())
	{
		UE_LOG(InteractionComponentLog, Display, TEXT("OnInteractionCompBeginOverlap"));
		ActorToInteract = OtherActor;

		ShowInteractMessage();
	}
}

void UWS_InteractionComponent::OnInteractionCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UWS_InteractableObject>())
	{
		if (!InProgress)
		{
			UE_LOG(InteractionComponentLog, Display, TEXT("OnInteractionCompEndOverlap"));
			ActorToInteract = nullptr;
			if (InteractMessage && InteractMessage->IsInViewport())
			{
				InteractMessage->RemoveFromParent();
				HoldingTime = 0.f;
				ActorInteractionType = EInteractionType::None;

				GetWorld()->GetTimerManager().ClearTimer(UpdatePercentHoldingTimerHandle);
				GetWorld()->GetTimerManager().ClearTimer(HoldingTimerHandle);
			}
		}
	}
}

void UWS_InteractionComponent::HideInteractMessage()
{
	if (ActorInteractionType == EInteractionType::Open)
	{/*
		GetWorld()->GetTimerManager().ClearTimer(UpdatePercentHoldingTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(HoldingTimerHandle);*/
		UE_LOG(InteractionComponentLog, Display, TEXT("HideInteractMessage | Open"));
	}
	if (InteractMessage && InteractMessage->IsInViewport())
	{
		UE_LOG(InteractionComponentLog, Display, TEXT("HideInteractMessage"));
		InteractMessage->RemoveFromParent();
		HoldingTime = 0.f;
		ActorInteractionType = EInteractionType::None;
	}
}

void UWS_InteractionComponent::ShowInteractMessage()
{
	UE_LOG(InteractionComponentLog, Display, TEXT("ShowInteractMessage"));
	GetWorld()->GetTimerManager().ClearTimer(HoldingTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(UpdatePercentHoldingTimerHandle);

	if (ActorToInteract)
	{
		if (!InteractMessage)
		{
			if (InteractMessageWidget)
			{
				InteractMessage = CreateWidget<UWS_InteractWidget>(GetWorld(), InteractMessageWidget);
			}
		}

		if (InteractMessage && !InteractMessage->IsInViewport())
		{
			InteractMessage->AddToViewport();
		}
	}
}

void UWS_InteractionComponent::PreExecuteInteract()
{
	if (ActorToInteract)
	{
		if (ActorInteractionType == EInteractionType::Loot)
		{
			UE_LOG(InteractionComponentLog, Display, TEXT("ExecuteInteract | Loot"));
			
		}
		else if (ActorInteractionType == EInteractionType::Open)
		{
			UE_LOG(InteractionComponentLog, Display, TEXT("ExecuteInteract | Open"));
			
		}
		GetWorld()->GetTimerManager().ClearTimer(UpdatePercentHoldingTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(HoldingTimerHandle);
		HideInteractMessage();
		IWS_InteractableObject::Execute_Interact(ActorToInteract, GetOwner());
		InProgress = false;
	}
}

void UWS_InteractionComponent::UpdatePercent()
{
	InteractMessage->SetTimePercent(HoldingTime - GetWorld()->GetTimerManager().GetTimerRemaining(HoldingTimerHandle));
}
