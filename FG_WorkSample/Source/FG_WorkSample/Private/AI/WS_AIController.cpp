// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/WS_AIController.h"
#include "AI/WS_AICharacter.h"
#include "Components/WS_AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AWS_AIController::AWS_AIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UWS_AIPerceptionComponent>("AIPerceptionComponent");
	SetPerceptionComponent(*AIPerceptionComponent);
}

ETeamAttitude::Type AWS_AIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	ETeamAttitude::Type Attitude;
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	if (OtherTeamAgent)
	{
		Attitude = FGenericTeamId::GetAttitude(GetGenericTeamId(), OtherTeamAgent->GetGenericTeamId());
	}
	else
	{
		Attitude = ETeamAttitude::Neutral;
	}
	return Attitude;
}

void AWS_AIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	IGenericTeamAgentInterface* ControlledAgent = Cast<IGenericTeamAgentInterface>(GetPawn());
	if (ControlledAgent)
	{
		ControlledAgent->SetGenericTeamId(NewTeamID);
	}
}

FGenericTeamId AWS_AIController::GetGenericTeamId() const
{
	const IGenericTeamAgentInterface* ControlledAgent = Cast<const IGenericTeamAgentInterface>(GetPawn());
	if (ControlledAgent)
	{
		return ControlledAgent->GetGenericTeamId();
	}

	return FGenericTeamId();
}

void AWS_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto AICharacter = Cast<AWS_AICharacter>(InPawn);
	if(AICharacter)
	{
		RunBehaviorTree(AICharacter->BehaviorTreeAsset);
	}
}

void AWS_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);*/
}

AActor* AWS_AIController::GetFocusOnActor() const
{
	if(!GetBlackboardComponent()) return nullptr;

	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
