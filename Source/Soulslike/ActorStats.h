// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_API UActorStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorStats();

private:
	UPROPERTY(EditAnywhere)
	float _maxHealth = 100;
	UPROPERTY(EditAnywhere)
	float _health = 80;

private:
	void Die();
	
public:
	UFUNCTION(BlueprintCallable)
	void IncreaseHeath(float value);
	UFUNCTION(BlueprintCallable)
	void DecreaseHeath(float value);
	float GetHeath() const { return _health;  };
};
