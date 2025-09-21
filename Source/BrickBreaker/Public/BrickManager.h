// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Brick.h"
#include "Ball.h"
#include "PowerUp.h"
#include "GameFramework/Actor.h"
#include "BrickManager.generated.h"

UCLASS()
class BRICKBREAKER_API ABrickManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABrickManager();

	UPROPERTY(EditAnywhere, Category = "Brick Grid")
	int32 Rows = 3;

	UPROPERTY(EditAnywhere, Category = "Brick Grid")
	int32 Columns = 8;

	UPROPERTY()
	TArray<ABrick*> Bricks;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABrick> BrickClass;
	void SpawnBricks(const FVector2D& AreaMin, const FVector2D& AreaMax);
	void RemoveBrick(ABrick* Brick);
	void ResetBall();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	TSubclassOf<APowerUp> PowerUpClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
    TSubclassOf<ABall> BallClass;
	
};