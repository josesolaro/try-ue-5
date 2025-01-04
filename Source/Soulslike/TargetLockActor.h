// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "TargetLockActor.generated.h"

UCLASS()
class SOULSLIKE_API ATargetLockActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetLockActor();

// protected:
// 	// Called when the game starts or when spawned
// 	virtual void BeginPlay() override;
//
// public:	
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;
private:
	UWidgetComponent* _widgetComp;
	
};
