// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}
void UCombatComponent::BeginPlay()
{
	this->_charMovementComponent = this->GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
	this->_cameraComponent = this->GetOwner()->GetComponentByClass<UCameraComponent>();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (this->_lockedTarget != nullptr)
	{
		if (this->_charMovementComponent != nullptr)
		{
			this->_charMovementComponent->bOrientRotationToMovement = false;
			this->_charMovementComponent->bUseControllerDesiredRotation = true;
		}
		this->_lockedTarget->GetComponentByClass<UWidgetComponent>()->SetVisibility(true);
		FRotator newRotation = (this->_lockedTarget->GetActorLocation() - this->GetOwner()->GetActorLocation()).Rotation();
		newRotation = newRotation.Add(-20, 0, 0);
		this->GetOwner()->GetInstigatorController()->SetControlRotation(newRotation);
	}
}


void UCombatComponent::TargetLock()
{
	if (this->_lockedTarget == nullptr)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
		objectTypesArray.Add(EObjectTypeQuery::ObjectTypeQuery1);

		TArray<AActor*> actorsToIgnore;

	
		FVector start = this->GetOwner()->GetActorLocation();
		FRotator actorRotation = this->_cameraComponent->GetComponentRotation();
		actorRotation.SetComponentForAxis(EAxis::Y, 0);

		FVector lookAt = actorRotation.Vector();
		lookAt = lookAt * this->TargetLockDistance;

		FVector end = start + lookAt;
		FHitResult hitResult;

		FCollisionShape sphere = FCollisionShape::MakeSphere(50.0);

		DrawDebugLine(GetWorld(),start, end, FColor::Red, false, 2);
	
		bool hit = GetWorld()->SweepSingleByChannel(hitResult, start, end, FQuat::Identity, ECC_GameTraceChannel1, sphere);
		if (hit)
		{
			this->_lockedTarget = hitResult.GetActor();
		}
	}
	else
	{
		this->_lockedTarget->GetComponentByClass<UWidgetComponent>()->SetVisibility(false);
		this->_lockedTarget = nullptr;
		this->_charMovementComponent->bOrientRotationToMovement = true;
		this->_charMovementComponent->bUseControllerDesiredRotation = false;
	}
	
	
}
