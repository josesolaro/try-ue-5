// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCombatComponent::TargetLock()
{

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
	objectTypesArray.Add(EObjectTypeQuery::ObjectTypeQuery1);

	TArray<AActor*> actorsToIgnore;
	
	UE_LOG(LogTemp, Display, TEXT("UCombatComponent::TargetLock"));
	FVector actorLocation = GetOwner()->GetActorLocation();
	UE_LOG(LogTemp, Display, TEXT("position: %f, %f, %f"),actorLocation.X, actorLocation.Y, actorLocation.Z);
	FRotator actorRotation = GetOwner()->GetActorRotation();
	FVector lookAt = actorRotation.Vector() * 1000.0f;
	FHitResult hitResult;
	bool hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		actorLocation,
		actorLocation + lookAt,
		50.0f,
		objectTypesArray,
		false,
		actorsToIgnore,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true);
	UE_LOG(LogTemp, Display, TEXT("HIT: %d"), hit);
}
