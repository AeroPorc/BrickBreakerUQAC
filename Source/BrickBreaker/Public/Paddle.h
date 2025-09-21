// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"

class UInputMappingContext;
class UInputAction;
#include "Components/StaticMeshComponent.h"
#include "Paddle.generated.h"

UCLASS()
class BRICKBREAKER_API APaddle : public APawn
{
	GENERATED_BODY()

public:
	APaddle();

	// Step size for discrete movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paddle")
	float StepSize = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Paddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_MoveLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_MoveRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paddle")
	UStaticMeshComponent* PaddleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paddle")
	float MovementSpeed = 1000.0f;

	// Input axis value
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paddle")
	float MoveDirection = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paddle|Bounds")
	float MinY = -500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paddle|Bounds")
	float MaxY = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paddle|Movement")
	float Acceleration = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Paddle|Movement")
	float Deceleration = 4000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Paddle|Movement")
	float CurrentVelocity = 0.0f;
	
protected:

		virtual void BeginPlay() override;

		void MoveRight(float Value);

		void MoveRightDiscrete(const FInputActionValue& Value);

		void MoveRightContinuous(const FInputActionValue& Value);

		void MoveRightStop(const FInputActionValue& Value);

		void MoveLeftContinuous(const FInputActionValue& Value);
		
		void MoveLeftDiscrete(const FInputActionValue& Value);
		
		void MoveLeftStop(const FInputActionValue& Value);
public:	
		virtual void Tick(float DeltaTime) override;

		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
