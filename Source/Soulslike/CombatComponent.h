// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TraceWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponAttached);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOULSLIKE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	AActor* _lockedTarget;
	class UCharacterMovementComponent* _charMovementComponent;
	UCameraComponent* _cameraComponent;
	UAnimInstance* _animationInstance;
	bool _isAttacking = false;
	bool _continueCombo = false;
	float _comboBoost = 1.0f;
	FTimerHandle _attackTraceTimer;
	UTraceWeaponComponent* _weapon;
	bool _weaponAttached = false;
	float _damage = 0.0f;


	void StopTraceWeapon();
	void StopTargetLock();

	UFUNCTION()
	void DoneAttackingAnimationEnd(UAnimMontage* montage, bool bInterrupted);

	UFUNCTION()
	void AttackingNotifiedBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void TraceWeaponForContact();

public:
	UFUNCTION(BlueprintCallable, Category="Combat")
	void TargetLock();

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool Attack();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void Dodge();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void WeaponAttached(float damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float TargetLockDistance = 1000.0f;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponAttached OnWeaponAttached;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float AttackTraceTimerRate = 0.5f;

	

	//ANIMATIONS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	UAnimMontage* DodgeForwardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	UAnimMontage* DodgeBackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	UAnimMontage* DodgeLeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	UAnimMontage* DodgeRigthMontage;
};
