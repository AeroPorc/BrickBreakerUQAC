// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickManager.h"
#include "Ball.h"
#include "Brick.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABrickManager::ABrickManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABrickManager::BeginPlay()
{
	Super::BeginPlay();

	SpawnBricks(FVector2D(-500,200), FVector2D(500,400));	
}

void ABrickManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> FoundBricks;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABrick::StaticClass(), FoundBricks);

	if (FoundBricks.Num() == 0)
	{
		SpawnBricks(FVector2D(-500,200), FVector2D(500,400));
	}
}

void ABrickManager::SpawnBricks(const FVector2D& AreaMin, const FVector2D& AreaMax)
{
	Bricks.Empty();
	float BrickWidth = (AreaMax.X - AreaMin.X) / Columns;
	float BrickHeight = (AreaMax.Y - AreaMin.Y) / Rows;

	FVector GridCenter(-200.0f, -110.0f, 510.0f);
	float TotalWidth = Columns * BrickWidth;
	float TotalHeight = Rows * BrickHeight;
	FVector GridOrigin = GridCenter - FVector(0.0f, TotalWidth / 2.0f, TotalHeight / 2.0f);

	for (int32 Row = 0; Row < Rows; Row++)
	{
		for (int32 Column = 0; Column < Columns; Column++)
		{
			FVector Location(
				GridOrigin.X,
				GridOrigin.Y + Column * BrickWidth + BrickWidth / 2,
				GridOrigin.Z + Row * BrickHeight + BrickHeight / 2
			);
			ABrick* Brick = GetWorld()->SpawnActor<ABrick>(BrickClass, Location, FRotator(0.0f, 90.0f, 0.0f));
			if (Brick)
			{
				Bricks.Add(Brick);
				Brick->Level = FMath::RandRange(1, 3);
				Brick->UpdateColour();
			}
		}
	}
}
void ABrickManager::ResetBall()
{
	TArray<AActor*> Balls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall::StaticClass(), Balls);
	if (Balls.Num() > 0)
	{
		ABall* Ball = Cast<ABall>(Balls[0]);
		if (Ball)
		{
			Ball->SetActorLocation(FVector(-200.0f, 0.0f, 400.0f));
			Ball->BallMovement->Velocity = FVector(0.0f, FMath::FRandRange(-200.0f, 200.0f), -Ball->BallMovement->InitialSpeed);
		}
	}
}
void ABrickManager::RemoveBrick(ABrick* Brick)
{
	Bricks.Remove(Brick);
	if (Bricks.Num() == 0)
	{
		ResetBall();
		SpawnBricks(FVector2D(-500,200), FVector2D(500,400));
	}
}