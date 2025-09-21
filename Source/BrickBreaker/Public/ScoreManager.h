// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreManager.generated.h"

UCLASS()
class BRICKBREAKER_API AScoreManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AScoreManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTextRenderComponent* ScoreText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Score;

	UFUNCTION(BlueprintCallable)
	void AddScore(int32 Points);

	void UpdateScore();
	
	UPROPERTY(EditAnywhere)
    FLinearColor JuicyColour = FLinearColor::Yellow;
    
    UPROPERTY(EditAnywhere)
    float JuicyScale = 2.0f;
    
    UPROPERTY(EditAnywhere)
    float JuicyDuration = 0.2f;
    
    FTimerHandle JuicyTimerHandle;
    FVector OriginalScale;
    FLinearColor OriginalColour;
    
    void TriggerJuicyEffect();
    void ResetJuicyEffect();

private:
	float PulseElapsed;
	float PulseFrequency;
	FTimerHandle PulseTimerHandle;

	void PulseEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
