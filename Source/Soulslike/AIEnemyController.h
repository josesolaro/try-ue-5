// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SOULSLIKE_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()

private:
	UPawnSensingComponent* _pawnSensingComp;

	UFUNCTION()
	void OnPawnSaw(APawn* sawPawn);
	
protected:
	virtual void BeginPlay() override;

public:
	AAIEnemyController();
	
	UBehaviorTree* _behaviorTree;
};
