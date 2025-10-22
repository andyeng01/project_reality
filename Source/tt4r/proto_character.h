// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "proto_character.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class TT4R_API Aproto_character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Aproto_character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Inputs and their functions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* SprintAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void Die();

	// components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* proto_camera_comp;

	// character variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties")
	float proto_hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties")
	float WalkSpeed;

private:
	bool bIsSprinting;
	float DefaultMaxWalkSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
