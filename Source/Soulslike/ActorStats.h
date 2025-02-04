// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, float, percentaje);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChange, float, percentaje);

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

	UPROPERTY(EditAnywhere)
	float _maxStamina = 100;
	UPROPERTY(EditAnywhere)
	float _stamina = 100;

private:
	void Die();
	void BeginPlay() override;
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaChange OnStaminaChange;
	
	UFUNCTION(BlueprintCallable)
	void IncreaseHeath(float value);
	UFUNCTION(BlueprintCallable)
	void DecreaseHeath(float value);

	void IncreaseStamina(float value);
	void DecreaseStamina(float value);

	UFUNCTION(BlueprintCallable)
	float GetPercentageHealt() const { return _health / _maxHealth;  };
	UFUNCTION(BlueprintCallable)
	float GetPercentageStamina() const { return _stamina / _maxStamina;  };
	
	
	float GetHeath() const { return _health;  };
	float GetStamina() const { return _stamina;  };
};
