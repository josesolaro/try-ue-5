// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoulslikeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ASoulslikeCharacter

ASoulslikeCharacter::ASoulslikeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// DamageSound = CreateDefaultSubobject<USoundBase>(TEXT("DamageSound"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


}

void ASoulslikeCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->CombatComponent = FindComponentByClass<UCombatComponent>();
	this->ActorStatsComponent = FindComponentByClass<UActorStats>();
	SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();

	SetupPlayerInputComponent(FindComponentByClass<UInputComponent>());
	this->OnTakeAnyDamage.AddUniqueDynamic(this, &ASoulslikeCharacter::OnDamageTake);

	StaminaDelegate.BindUFunction(this, FName("ReplenishStamina"));
	GetWorldTimerManager().SetTimer(TimerHandle_StaminaDelay, StaminaDelegate, 0.5f, true);
}


//////////////////////////////////////////////////////////////////////////
// Input

void ASoulslikeCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASoulslikeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->ClearActionBindings();
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoulslikeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoulslikeCharacter::Look);

		EnhancedInputComponent->BindAction(LockAction, ETriggerEvent::Started, this->CombatComponent, &UCombatComponent::TargetLock);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASoulslikeCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ASoulslikeCharacter::Dodge);
		EnhancedInputComponent->BindAction(PotionAction, ETriggerEvent::Completed, this, &ASoulslikeCharacter::UseHealthPotion);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASoulslikeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASoulslikeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASoulslikeCharacter::OnDamageTake(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DamageSound, GetActorLocation());
	this->ActorStatsComponent->DecreaseHeath(Damage);
}

void ASoulslikeCharacter::Attack()
{
	if (this->ActorStatsComponent->GetStamina() >= 10 && this->CombatComponent->Attack())
	{
		this->ActorStatsComponent->DecreaseStamina(10);
	}
}
void ASoulslikeCharacter::Dodge()
{
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		return;
	}
	if (this->ActorStatsComponent->GetStamina() >= 20)
	{
		this->ActorStatsComponent->DecreaseStamina(20);
		this->CombatComponent->Dodge();
	}
}

void ASoulslikeCharacter::ReplenishStamina()
{
	this->ActorStatsComponent->IncreaseStamina(5);
}

void ASoulslikeCharacter::PickWeapon_Implementation(AActor* weaponActor, FName socket, float damage)
{
	weaponActor->GetComponentByClass<UCapsuleComponent>()->OnComponentBeginOverlap.Clear();
	SkeletalMesh->GetSocketByName(socket)->AttachActor(weaponActor, SkeletalMesh);
	CombatComponent->WeaponAttached(damage);
}

void ASoulslikeCharacter::UseHealthPotion()
{
	if (_healthPotionCount<=0)
	{
		return;
	}
	_healthPotionCount -= 1;
	ActorStatsComponent->IncreaseHeath(20);
}

