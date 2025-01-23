// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidgetComponent.h"

#include "Components/ProgressBar.h"

// Sets default values for this component's properties
UPlayerWidgetComponent::UPlayerWidgetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UPlayerWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	this->HealthUserWidget = CreateWidget<UUserWidget>(GetOwner()->GetGameInstance(), HealthUserWidgetClass);
	if (this->HealthUserWidget != nullptr)
	{
		this->HealthUserWidget->AddToViewport();

		//work for now
		HealthUserWidget->SetRenderTranslation(FVector2d(-1000,-450));
	}
}
