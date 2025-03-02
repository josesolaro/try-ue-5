// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "ActorStats.h"
#include "EnhancedInputComponent.h"
#include "TraceWeaponComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
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
	Super::BeginPlay();
	this->_charMovementComponent = this->GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
	this->_cameraComponent = this->GetOwner()->GetComponentByClass<UCameraComponent>();

	TArray<AActor*> Overlaps;
	this->_weapon = this->GetOwner()->GetComponentByClass<UTraceWeaponComponent>();
	
	this->_animationInstance = this->GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	if (this->_lockedTarget != nullptr)
	{
		if (this->_charMovementComponent != nullptr)
		{
			this->_charMovementComponent->bOrientRotationToMovement = false;
			this->_charMovementComponent->bUseControllerDesiredRotation = true;
		}
		for (auto component : this->_lockedTarget->GetComponents())
		{
			UWidgetComponent* widget = Cast<UWidgetComponent>(component);
			if (widget != nullptr)
			{
				widget->SetVisibility(true);
			}
		}
		FRotator newRotation = (this->_lockedTarget->GetActorLocation() - this->GetOwner()->GetActorLocation()).
			Rotation();
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
		TArray<FHitResult> hitResults;

		FCollisionShape sphere = FCollisionShape::MakeSphere(50.0);

		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2);

		bool hit = GetWorld()->SweepMultiByChannel(hitResults, start, end, FQuat::Identity, ECC_GameTraceChannel1,
		                                            sphere);

		if (hit)
		{
			for (int i = 0; i < hitResults.Num(); i++)
			{
				if (hitResults[i].GetActor() != this->GetOwner())
				{
					this->_lockedTarget = hitResults[i].GetActor();
					break;
				}
			}
		}

	}
	else
	{
		this->StopTargetLock();
	}
}

void UCombatComponent::Dodge()
{

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if (this->_lockedTarget == nullptr)
	{
		_animationInstance->Montage_Play(DodgeForwardMontage);
	}
	else
	{
		if (PlayerController->IsInputKeyDown(EKeys::W))
		{
			_animationInstance->Montage_Play(DodgeForwardMontage);
		}
		else if (PlayerController->IsInputKeyDown(EKeys::S))
		{
			_animationInstance->Montage_Play(DodgeBackMontage);
		}
		else if (PlayerController->IsInputKeyDown(EKeys::A))
		{
			_animationInstance->Montage_Play(DodgeLeftMontage);
		}
		else if (PlayerController->IsInputKeyDown(EKeys::D))
		{
			_animationInstance->Montage_Play(DodgeRigthMontage);
		}
	}
}

void UCombatComponent::WeaponAttached(float damage)
{
	_damage = damage;
	_weaponAttached = true;
	OnWeaponAttached.Broadcast();
}


bool UCombatComponent::Attack()
{
	if (_weaponAttached && !_isAttacking)
	{
		this->_animationInstance->Montage_Play(this->AttackMontage);

		this->_animationInstance->OnMontageEnded.AddUniqueDynamic(this, &UCombatComponent::DoneAttackingAnimationEnd);
		
		this->_animationInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UCombatComponent::AttackingNotifiedBegin);

		_isAttacking = true;
		this->_continueCombo = false;
		return true;
	}
	else if (!this->_continueCombo)
	{

		this->_continueCombo = true;
		return true;
	}

	return false;
}

void UCombatComponent::DoneAttackingAnimationEnd(UAnimMontage* montage, bool bInterrupted)
{
	_comboBoost = 1.0f;
	_isAttacking = false;
}

void UCombatComponent::AttackingNotifiedBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	auto notifyPlainName = NotifyName.GetPlainNameString();
	
	if (notifyPlainName.Compare("Combo") == 0)
	{
		if (!this->_continueCombo)
		{
			this->_animationInstance->Montage_Stop(1.0);
		}
		this->_continueCombo = false;
	}
	else if (notifyPlainName.Compare("WeaponTraceStart") == 0)
	{
		GetWorld()->GetTimerManager().SetTimer(this->_attackTraceTimer,this, &UCombatComponent::TraceWeaponForContact, this->AttackTraceTimerRate, true);
	}
	else if (notifyPlainName.Compare("WeaponTraceEnd") == 0){
		this->StopTraceWeapon();
	}
}

void UCombatComponent::TraceWeaponForContact()
{
	FHitResult hitResult;
	this->_weapon->GetHitted(hitResult);
	if (hitResult.GetActor() != nullptr && hitResult.GetActor() != this->GetOwner())
	{
		_comboBoost = _comboBoost * 1.1f;
		this->StopTraceWeapon();
		FDamageEvent damageEvent;
		hitResult.GetActor()->TakeDamage(_damage * _comboBoost, damageEvent,  this->GetOwner()->GetInstigatorController(), this->GetOwner());
		if (hitResult.GetActor()->GetComponentByClass<UActorStats>()->GetHeath() <= 0)
		{
			this->StopTargetLock();
		}
	}
}

void UCombatComponent::StopTraceWeapon()
{
	GetWorld()->GetTimerManager().ClearTimer(this->_attackTraceTimer);
}

void UCombatComponent::StopTargetLock()
{
	if (_lockedTarget != nullptr)
	{
		for (auto component : this->_lockedTarget->GetComponents())
		{
			UWidgetComponent* widget = Cast<UWidgetComponent>(component);
			if (widget != nullptr)
			{
				widget->SetVisibility(false);
			}
		}
		this->_lockedTarget = nullptr;
	}
	this->_charMovementComponent->bOrientRotationToMovement = true;
	this->_charMovementComponent->bUseControllerDesiredRotation = false;
}
