// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Brick.generated.h"

UCLASS()
class BRICKBREAKER_API ABrick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrick();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick")
	int32 Level;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BrickMesh;

	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	class UMaterialInterface* ParticleMaterial;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ParticleSystem;
	

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundBase* DestroyedSound;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Brick")
	void HandleHit(const FVector& HitLocation = FVector::ZeroVector);

	void UpdateColour();
};
