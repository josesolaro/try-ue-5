// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorStats.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsAsset.h"

// Sets default values for this component's properties
UActorStats::UActorStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}

void UActorStats::BeginPlay()
{
	Super::BeginPlay();
}


void UActorStats::IncreaseHeath(float value)
{
	_health += value;
	if (_health > _maxHealth)
	{
		_health = _maxHealth;
	}
	OnHealthChange.Broadcast(_health/_maxHealth);
}

void UActorStats::DecreaseHeath(float value)
{
	_health -= value;
	if (_health <= 0)
	{
		this->Die();
	}
	OnHealthChange.Broadcast(_health/_maxHealth);
}

void UActorStats::Die()
{
	GetOwner()->GetComponentByClass<UCharacterMovementComponent>()->DisableMovement();
	GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->SetSimulatePhysics(true);
}

void UActorStats::IncreaseStamina(float value)
{
	_stamina += value;
	if (_stamina > _maxStamina)
	{
		_stamina = _maxStamina;
	}
	OnStaminaChange.Broadcast(_stamina/_maxStamina);
}	

void UActorStats::DecreaseStamina(float value)
{
	_stamina -= value;
	OnStaminaChange.Broadcast(_stamina/_maxStamina);
}