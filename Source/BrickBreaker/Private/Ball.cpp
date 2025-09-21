// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include  "Paddle.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(15.0f);
	SphereComponent->SetCollisionProfileName("BlockAllDynamic");


	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(RootComponent);

	BallMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BallMovement"));
	BallMovement->InitialSpeed = 300.0f;
	BallMovement->MaxSpeed = 400.0f;
	BallMovement->bShouldBounce = true;
	BallMovement->Bounciness = 1.0f;
	BallMovement->Friction = 0.0f;
	BallMovement->ProjectileGravityScale = 0.0f;

	SphereComponent-> OnComponentHit.AddDynamic(this, &ABall::OnHit);

}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	float RandomImpulseY = FMath::FRandRange(-200.0f, 200.0f);
	BallMovement->Velocity = FVector(0.0f, RandomImpulseY, -BallMovement->InitialSpeed);}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	FVector Location = GetActorLocation();
	Location.X = -200.0f;
	SetActorLocation(Location);
	float CurrentSpeed = BallMovement->Velocity.Size();
	if (CurrentSpeed > KINDA_SMALL_NUMBER && !FMath::IsNearlyEqual(CurrentSpeed, BallMovement->InitialSpeed))
	{
		BallMovement->Velocity = BallMovement->Velocity.GetSafeNormal() * BallMovement->InitialSpeed;
	}
	float LoseZ = 55.0f;
	if (GetActorLocation().Z < LoseZ)
	{
		TArray<AActor*> Balls;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall::StaticClass(), Balls);
		if (Balls.Num() > 1)
		{
			Destroy();
		}
		else
		{
			BallMovement->StopMovementImmediately();
			BallMovement->SetActive(false);

			TArray<AActor*> FoundPaddles;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaddle::StaticClass(), FoundPaddles);
			if (FoundPaddles.Num() > 0)
			{
				APaddle* Paddle = Cast<APaddle>(FoundPaddles[0]);
				if (Paddle)
				{
					if (APlayerController* PC = Cast<APlayerController>(Paddle->GetController()))
					{
						Paddle->DisableInput(PC);
					}
				}
			}

			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				if (GameOverWidgetClass) 
				{
					UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(PC, GameOverWidgetClass);
					if (GameOverWidget)
					{
						GameOverWidget->AddToViewport();
					}
				}
			}

			Destroy();
		}
	}
		
}

void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //Depricated code TBH not sure why I kept it here. "Felt cute might delete later" type thing. 
	// if (OtherActor && OtherActor->IsA(APaddle::StaticClass()))
	// {
	// 	FVector BallLocation = GetActorLocation();
	// 	FVector PaddleLocation = OtherActor->GetActorLocation();
	// 	float HitOffset = BallLocation.Y - PaddleLocation.Y;
	//
	// 	BallMovement->Velocity *= -1;
	//
	// 	if (FMath::Abs(HitOffset) < 1.0f)
	// 	{
	// 		BallMovement->Velocity.Y += 30.0f * (FMath::RandBool() ? 1 : -1);
	// 	}
	// 	else
	// 	{
	// 		BallMovement->Velocity.Y += FMath::Clamp(HitOffset * 2.0f, -BallMovement->InitialSpeed * 10.0f, BallMovement->InitialSpeed * 0.5f);
	// 	}
	// }

	if (BounceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
	}
	UE_LOG(LogTemp, Warning, TEXT("Ball hit %s"), *OtherActor->GetName());

}

