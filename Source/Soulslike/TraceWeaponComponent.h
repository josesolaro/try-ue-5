// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/ArrowComponent.h"
#include "TraceWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_API UTraceWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTraceWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UArrowComponent* _start;
	UArrowComponent* _end;

public:	
	bool GetHitted(FHitResult& hitResult);
		
};
