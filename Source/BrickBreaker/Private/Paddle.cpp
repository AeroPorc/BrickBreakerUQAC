// Fill out your copyright notice in the Description page of Project Settings.


#include "Paddle.h"
#include "TimerManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

// Sets default values
APaddle::APaddle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PaddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleMesh"));
	PaddleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PaddleMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = PaddleMesh;

	
}

// Called when the game starts or when spawned
void APaddle::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (IMC_Paddle)
			{
				Subsystem->AddMappingContext(IMC_Paddle, 0);
			}
		}
	}
}


// Called every frame
void APaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FMath::Abs(MoveDirection) > KINDA_SMALL_NUMBER)
	{
		CurrentVelocity = FMath::FInterpTo(CurrentVelocity, MoveDirection * MovementSpeed, DeltaTime, Acceleration * DeltaTime);
	}
	else
	{
		CurrentVelocity = FMath::FInterpTo(CurrentVelocity, 0.0f, DeltaTime, Deceleration * DeltaTime);
	}

	FVector NewLocation = GetActorLocation();
	NewLocation.Y += CurrentVelocity * DeltaTime;
	NewLocation.Y = FMath::Clamp(NewLocation.Y, MinY, MaxY);
	SetActorLocation(NewLocation);
}

void APaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_MoveLeft)
		{
			EnhancedInput->BindAction(IA_MoveLeft, ETriggerEvent::Triggered, this, &APaddle::MoveLeftContinuous);
			EnhancedInput->BindAction(IA_MoveLeft, ETriggerEvent::Started, this, &APaddle::MoveLeftDiscrete);
			EnhancedInput->BindAction(IA_MoveLeft, ETriggerEvent::Completed, this, &APaddle::MoveLeftStop);
		}
		if (IA_MoveRight)
		{
			EnhancedInput->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &APaddle::MoveRightContinuous);
			EnhancedInput->BindAction(IA_MoveRight, ETriggerEvent::Started, this, &APaddle::MoveRightDiscrete);
			EnhancedInput->BindAction(IA_MoveRight, ETriggerEvent::Completed, this, &APaddle::MoveRightStop);
		}
	}
}

void APaddle::MoveLeftContinuous(const FInputActionValue& Value)
{
	MoveDirection = 1.0f;
}

void APaddle::MoveLeftDiscrete(const FInputActionValue& Value)
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Y += StepSize;
	SetActorLocation(NewLocation);
}

void APaddle::MoveLeftStop(const FInputActionValue& Value)
{
	MoveDirection = 0.0f;
}

void APaddle::MoveRightStop(const FInputActionValue& Value)
{
	MoveDirection = 0.0f;
}

void APaddle::MoveRightDiscrete(const FInputActionValue& Value)
{
	FVector NewLocation = GetActorLocation();
    NewLocation.Y -= StepSize;
    SetActorLocation(NewLocation);
}

void APaddle::MoveRightContinuous(const FInputActionValue& Value)
{
	MoveDirection = -1.0f;
}

void APaddle::MoveRight(float Value)
{
	MoveDirection = Value;

}

