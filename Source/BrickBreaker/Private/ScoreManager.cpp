// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreManager.h"
#include "Components/TextRenderComponent.h"
#include "TimerManager.h"


// Sets default values
AScoreManager::AScoreManager()
{
	PrimaryActorTick.bCanEverTick = false;
	Score = 0;
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText"));
	RootComponent = ScoreText;
	JuicyColour = FLinearColor(1.0f, 0.5f, 0.0f, 1.0f);
	JuicyScale = 3.0f;
	JuicyDuration = 0.5f;
	OriginalScale = ScoreText->GetComponentScale();
	OriginalColour = FLinearColor::White;
	UpdateScore();
}

// Called when the game starts or when spawned
void AScoreManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScoreManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AScoreManager::AddScore(int32 Amount)
{
	Score += Amount;
	UpdateScore();
	TriggerJuicyEffect();
}

void AScoreManager::UpdateScore()
{
	ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
}


void AScoreManager::TriggerJuicyEffect()
{
	PulseElapsed = 0.0f;
	PulseFrequency = 10.0f;
	GetWorld()->GetTimerManager().SetTimer(PulseTimerHandle, this, &AScoreManager::PulseEffect, 0.016f, true); 
	GetWorld()->GetTimerManager().SetTimer(JuicyTimerHandle, this, &AScoreManager::ResetJuicyEffect, JuicyDuration, false);
	ScoreText->SetWorldScale3D(OriginalScale * JuicyScale);
	ScoreText->SetTextRenderColor(JuicyColour.ToFColor(true));

	GetWorld()->GetTimerManager().SetTimer(JuicyTimerHandle, this, &AScoreManager::ResetJuicyEffect, JuicyDuration, false);
}

void AScoreManager::ResetJuicyEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(PulseTimerHandle);
	ScoreText->SetWorldScale3D(OriginalScale);
	ScoreText->SetTextRenderColor(OriginalColour.ToFColor(true));
}

void AScoreManager::PulseEffect()
{
	
	PulseElapsed += 0.016f;
	float Alpha = FMath::Sin(PulseElapsed * PulseFrequency * PI) * 0.5f + 0.5f; // 0..1
	FVector NewScale = FMath::Lerp(OriginalScale, OriginalScale * JuicyScale, Alpha);
	FLinearColor NewColor = FMath::Lerp(OriginalColour, JuicyColour, Alpha);
	ScoreText->SetWorldScale3D(NewScale);
	ScoreText->SetTextRenderColor(NewColor.ToFColor(true));
}