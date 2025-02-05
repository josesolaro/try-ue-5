// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TraceWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AActor* _lockedTarget;
	class UCharacterMovementComponent* _charMovementComponent;
	UCameraComponent* _cameraComponent;
	UAnimInstance* _animationInstance;
	bool _isAttacking = false;
	bool _continueCombo = false;
	FTimerHandle _attackTraceTimer;
	UTraceWeaponComponent* _weapon;
	

	void StopTraceWeapon();
	void StopTargetLock();
	
	UFUNCTION()
	void DoneAttackingAnimationEnd(UAnimMontage* montage, bool bInterrupted);

	UFUNCTION()
	void AttackingNotifiedBegin( FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void TraceWeaponForContact();
	
public:
	UFUNCTION(BlueprintCallable, Category="Combat")
	void TargetLock();

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool Attack();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void Dodge();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float TargetLockDistance = 1000.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float AttackTraceTimerRate = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float Damage = 10.0f;

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
