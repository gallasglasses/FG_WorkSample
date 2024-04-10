// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/WS_InteractableObject.h"


void IWS_InteractableObject::NotifyInteractionFinished(AActor* InteractableObject, AActor* ActorInteractedWithObject)
{
	if (OnInteractionFinished.IsBound())
	{
		OnInteractionFinished.Broadcast(InteractableObject, ActorInteractedWithObject);
	}
}