// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

UENUM(BlueprintType)
enum class EPowerUpType : uint8
{
	LowerBrickLevel,
	SpawnExtraBalls
};

UCLASS()
class BRICKBREAKER_API APowerUp : public AActor
{
	GENERATED_BODY()
public:
	APowerUp();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	float FallSpeed = 200.0f;
	
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPowerUpType PowerUpType;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Collision;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
				   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
				   bool bFromSweep, const FHitResult& SweepResult);
};