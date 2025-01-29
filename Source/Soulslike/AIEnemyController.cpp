// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIEnemyController::AAIEnemyController()
{
	_pawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	ConstructorHelpers::FObjectFinderOptional<UBehaviorTree> behaviorTreeRef(TEXT("/Game/AI/BT_Enemy.BT_Enemy"));
	_behaviorTree = behaviorTreeRef.Get();
}
void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(_behaviorTree);
	if (_pawnSensingComp != nullptr)
	{
		_pawnSensingComp->OnSeePawn.AddUniqueDynamic(this, &AAIEnemyController::OnPawnSaw);
		
		_pawnSensingComp->bHearNoises = false;
		_pawnSensingComp->SightRadius = 50000;
		_pawnSensingComp->SensingInterval = 0.2f;
		_pawnSensingComp->SetPeripheralVisionAngle(85.0f);
	}
	
}

void AAIEnemyController::OnPawnSaw(APawn* sawPawn)
{
	if (GetBlackboardComponent() != nullptr)
	{
		GetBlackboardComponent()->SetValueAsObject("Target", sawPawn);
	}
}
