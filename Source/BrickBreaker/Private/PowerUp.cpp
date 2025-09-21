// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "Paddle.h"
#include "BrickManager.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetSphereRadius(30.0f);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnOverlap);
}

void APowerUp::BeginPlay()
{
	Super::BeginPlay();
}

void APowerUp::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						 bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(APaddle::StaticClass()))
	{
		if (PowerUpType == EPowerUpType::LowerBrickLevel)
		{
			TArray<AActor*> Bricks;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABrick::StaticClass(), Bricks);
			for (AActor* BrickActor : Bricks)
			{
				ABrick* Brick = Cast<ABrick>(BrickActor);
				Brick->Level -= 1;
				Brick->UpdateColour();
			}
		}
		else if (PowerUpType == EPowerUpType::SpawnExtraBalls)
		{
			TArray<AActor*> FoundManagers;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABrickManager::StaticClass(), FoundManagers);
			TSubclassOf<ABall> BallClass = ABall::StaticClass();
			if (FoundManagers.Num() > 0)
			{
				ABrickManager* Manager = Cast<ABrickManager>(FoundManagers[0]);
				if (Manager && Manager->BallClass)
				{
					BallClass = Manager->BallClass;
				}
			}
			for (int i = 0; i < 3; ++i)
			{
				FVector SpawnLoc = FVector(-200.0,-110.0f, 260.0f);
				GetWorld()->SpawnActor<ABall>(BallClass, SpawnLoc, FRotator::ZeroRotator);
			}
		}
		Destroy();
	}
}

void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Location = GetActorLocation();
	Location.Z -= FallSpeed * DeltaTime; 
	SetActorLocation(Location);
}