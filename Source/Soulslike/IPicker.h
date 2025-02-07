// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPicker.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UIPicker : public UInterface
{
	GENERATED_BODY()
	
};

class IIPicker
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PickWeapon(AActor* weaponActor, FName socket, float damage);
	
};