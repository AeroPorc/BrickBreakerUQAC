// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick.h"
#include "PowerUp.h"
#include "ScoreManager.h"
#include "BrickManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABrick::ABrick()
{
 	PrimaryActorTick.bCanEverTick = true;
	BrickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BrickMesh"));
	BrickMesh->SetWorldScale3D(FVector(0.2f,1.0f,0.2f));
	RootComponent = BrickMesh;

	BrickMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BrickMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	BrickMesh->OnComponentHit.AddDynamic(this, &ABrick::OnHit);
	DynMaterial = nullptr;
	
}

void ABrick::BeginPlay()
{
	Super::BeginPlay();
	if (ParticleMaterial)
	{
		DynMaterial = UMaterialInstanceDynamic::Create(ParticleMaterial, this);
	}
	UpdateColour();

}

void ABrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Level<=0)
	{
		if (DestroyedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DestroyedSound, GetActorLocation());
		}
		Destroy();
	}
}
void ABrick::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Brick was hit by %s"), *OtherActor->GetName());
	HandleHit(Hit.Location);
}
void ABrick::UpdateColour()
{
	FLinearColor Colour;
	switch (Level)
	{
		case 3: Colour = FLinearColor::Green; break;
		case 2: Colour = FLinearColor::Yellow; break;
		default: Colour = FLinearColor::Red; break;
	}
	BrickMesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(Colour));

}

void ABrick::HandleHit(const FVector& HitLocation)
{
	FLinearColor ParticleColor;
	switch (Level)
	{
	case 3: ParticleColor = FLinearColor::Green; break;
	case 2: ParticleColor = FLinearColor::Yellow; break;
	default: ParticleColor = FLinearColor::Red; break;
	}

	if (ParticleSystem)
	{
		FVector SpawnLocation = HitLocation.IsZero() ? GetActorLocation() : HitLocation;

		auto* PSC = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ParticleSystem,
			SpawnLocation,
			FRotator::ZeroRotator,
			true
		);

		if (PSC)
		{
			UMaterialInterface* OriginalMaterial = PSC->GetMaterial(0);
            
			if (OriginalMaterial)
			{
				UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(OriginalMaterial, PSC);
                
				if (DynamicMat)
				{
					DynamicMat->SetVectorParameterValue(TEXT("Colour"), ParticleColor);
					PSC->SetMaterial(0, DynamicMat);
				}
			}
		}
	}

	if (Level > 1)
	{
		Level--;
		UpdateColour();
	}
	else
	{
		TArray<AActor*> FoundManagers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScoreManager::StaticClass(), FoundManagers);
		if (FoundManagers.Num() > 0)
		{
			AScoreManager* ScoreManager = Cast<AScoreManager>(FoundManagers[0]);
			if (ScoreManager)
			{
				ScoreManager->AddScore(100);
			}
		}
		FoundManagers.Empty();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABrickManager::StaticClass(), FoundManagers);
		if (FoundManagers.Num() > 0)
		{
			ABrickManager* Manager = Cast<ABrickManager>(FoundManagers[0]);
			if (Manager)
			{
				Manager->RemoveBrick(this);
			}
		}
		if (DestroyedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DestroyedSound, GetActorLocation());
		}
		
		if (FMath::FRand() < 0.3f) 
		{
			TSubclassOf<APowerUp> PowerUpClass = APowerUp::StaticClass();
			EPowerUpType ChosenType = (FMath::RandBool()) ? EPowerUpType::LowerBrickLevel : EPowerUpType::SpawnExtraBalls;
			if (FoundManagers.Num() > 0)
			{
				ABrickManager* Manager = Cast<ABrickManager>(FoundManagers[0]);
				if (Manager && Manager->PowerUpClass)
				{
					PowerUpClass = Manager->PowerUpClass;
				}
			}
			APowerUp* PowerUp = GetWorld()->SpawnActor<APowerUp>(PowerUpClass, GetActorLocation(), FRotator::ZeroRotator);
			if (PowerUp)
			{
				PowerUp->PowerUpType = ChosenType;
			}
		}
		Destroy();
	}
}
