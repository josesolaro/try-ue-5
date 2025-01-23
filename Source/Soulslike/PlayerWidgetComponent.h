// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerWidgetComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_API UPlayerWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerWidgetComponent();

	UPROPERTY(EditAnywhere, Blueprintable, Category = CustomWidget)
	TSubclassOf<UUserWidget> HealthUserWidgetClass;
	
	UUserWidget* HealthUserWidget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
