// Fill out your copyright notice in the Description page of Project Settings.


#include "TraceWeaponComponent.h"

// Sets default values for this component's properties
UTraceWeaponComponent::UTraceWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	auto arrowComponents = this->GetOwner()->GetComponents();
	for (auto component : arrowComponents)
	{
		if (component->GetName() == "Start")
		{
			this->_start = Cast<UArrowComponent>(component);
		}
		if (component->GetName() == "End")
		{
			this->_end = Cast<UArrowComponent>(component);
		}
	}
}

bool UTraceWeaponComponent::GetHitted(FHitResult& hitResult)
{
	FCollisionShape shape = FCollisionShape::MakeSphere(50.0);
	DrawDebugLine(GetWorld(), this->_start->GetComponentLocation(), this->_end->GetComponentLocation(), FColor::Red, false, 2);
	return  GetWorld()->SweepSingleByChannel(hitResult, this->_start->GetComponentLocation(), this->_end->GetComponentLocation(), FQuat::Identity, ECC_GameTraceChannel1, shape);
}
