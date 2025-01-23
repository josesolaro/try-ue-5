// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWidgetComponent.h"
#include "Components/ActorComponent.h"
#include "ActorStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, float, percentaje);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_API UActorStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorStats();

private:
	UPlayerWidgetComponent* PlayerWidget;
	
	UPROPERTY(EditAnywhere)
	float _maxHealth = 100;
	UPROPERTY(EditAnywhere)
	float _health = 80;

private:
	void Die();
	void BeginPlay() override;
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChange;
	
	UFUNCTION(BlueprintCallable)
	void IncreaseHeath(float value);
	UFUNCTION(BlueprintCallable)
	void DecreaseHeath(float value);
	UFUNCTION(BlueprintCallable)
	float GetPercentageHealt() const { return _health / _maxHealth;  };
	
	float GetHeath() const { return _health;  };
};
