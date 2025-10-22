// Fill out your copyright notice in the Description page of Project Settings.


#include "proto_character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
Aproto_character::Aproto_character()
{
	PrimaryActorTick.bCanEverTick = true;

	// create the collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	// create the first person camera
	proto_camera_comp = CreateDefaultSubobject<UCameraComponent>(TEXT("proto_camera"));
	proto_camera_comp->SetupAttachment(GetCapsuleComponent());

	// position the camera at eye level
	proto_camera_comp->SetRelativeLocation(FVector(10.f, 0.f, 64.f));
	proto_camera_comp->bUsePawnControlRotation = true;

	// set rotation based on camera rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GravityScale = 1.f;

	// walk and sprint speed configuration
	SprintSpeedMultiplier = 1.5f;
	WalkSpeed = 650.f;
	bIsSprinting = false;
	DefaultMaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


// Called when the game starts or when spawned
void Aproto_character::BeginPlay()
{
	Super::BeginPlay();
	
	// add the input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void Aproto_character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Aproto_character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// cast PlayerInputComponent to Enhanced Input
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// bind actions to enhanced input component
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Aproto_character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Aproto_character::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &Aproto_character::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &Aproto_character::StopJumping);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &Aproto_character::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &Aproto_character::StopSprint);
	}
}

void Aproto_character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// TODO: add some debug statements here to see what the hell is going on
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void Aproto_character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void Aproto_character::StartJump(const FInputActionValue& Value)
{
	Jump();
}

void Aproto_character::StopJump(const FInputActionValue& Value)
{
	StopJumping();
}

void Aproto_character::StartSprint(const FInputActionValue& Value)
{
	if (!bIsSprinting)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SprintSpeedMultiplier;
	}
}

void Aproto_character::StopSprint(const FInputActionValue& Value)
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void Aproto_character::Die()
{

}

