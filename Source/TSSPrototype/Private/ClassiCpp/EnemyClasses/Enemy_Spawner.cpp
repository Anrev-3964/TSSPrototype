// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/EnemyClasses/Enemy_Spawner.h"

#include "Camera/CameraComponent.h"
#include "ClassiCpp/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy_Spawner::AEnemy_Spawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnRate = 10.0f; //How often the enemies spawn
	SpawnCount = 10; //Number of enemies spawning together at once
	SpawnedEnemies = 0;
	MaxEnemies = 100;
}

// Called when the game starts or when spawned
void AEnemy_Spawner::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		AttachToPlayer();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter not found in the world."));
	}

	SpawnLogic();
	// Set the timer to spawn enemies at intervals
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemy_Spawner::SpawnLogic, SpawnRate, true);
}

void AEnemy_Spawner::SpawnLogic()
{
	if (SpawnedEnemies >= MaxEnemies)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
		return;
	}

	int Index = 0;
	if (EnemyToSpawn.Num() > 0 && EnemyToSpawn[Index])
	{
		FRotator Rotator = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		if (SpawnCount > 0)
		//If the SpawnCount has a number different from 0 it gets used to choose how many enemies to spawn,
		//otherwise only a single enemy gets spawned
		{
			for (int i = 0; i < SpawnCount; i++)
			{
				FVector Location = GetPositionOutsideCamera();
				ACharacter* SpawnedEnemy = GetWorld()->SpawnActor<ACharacter>(
					EnemyToSpawn[Index], Location, Rotator, SpawnParameters);
				if (SpawnedEnemy)
				{
					UE_LOG(LogTemp, Log, TEXT("Spawned Enemy: %s"), *SpawnedEnemy->GetName());
					SpawnedEnemies++;
				}
			}
		}
		else
		{
			FVector Location = GetPositionOutsideCamera();
			ACharacter* SpawnedEnemy = GetWorld()->SpawnActor<ACharacter>(
				EnemyToSpawn[Index], Location, Rotator, SpawnParameters);
			if (SpawnedEnemy)
			{
				UE_LOG(LogTemp, Log, TEXT("Spawned Enemy: %s"), *SpawnedEnemy->GetName());
				SpawnedEnemies++;
			}
		}

		UE_LOG(LogTemp, Log, TEXT("SpawnedEnemies= %i"), SpawnedEnemies);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ENEMY NOT FOUND"));
	}
}

void AEnemy_Spawner::AttachToPlayer()
{
	if (PlayerCharacter)
	{
		UCapsuleComponent* PlayerCapsule = PlayerCharacter->GetCapsuleComponent();
		if (PlayerCapsule)
		{
			// Attach this actor to the capsule, so that the spawner always follows the player character
			AttachToComponent(PlayerCapsule, FAttachmentTransformRules::SnapToTargetIncludingScale);
			UE_LOG(LogTemp, Log, TEXT("Spawner successfully attached to Player Capsule."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Capsule Component is null."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter is null."));
	}
}

FVector AEnemy_Spawner::GetPositionOutsideCamera()
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter is null."));
		return GetActorLocation(); //Sets the default location to the spawner location
	}

	UCameraComponent* PlayerCamera = PlayerCharacter->FindComponentByClass<UCameraComponent>();
	if (!PlayerCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraComponent is null."));
		return GetActorLocation();
	}

	FVector CameraLocation = PlayerCamera->GetComponentLocation();
	FVector CameraForward = PlayerCamera->GetForwardVector();
	float SpawnRange = 2000.0f;
	//Setting a large range allows this script to spawn enemies outside the player's camera view

	FVector RandomDirection = FMath::VRand();
	RandomDirection.Z = 0.0f;
	RandomDirection = RandomDirection.GetSafeNormal();
	if (FVector::DotProduct(RandomDirection, CameraForward) > 0.5f)
	{
		RandomDirection *= -1.0f; // Flip the direction to ensure it is outside the camera's forward view
	}

	FVector SpawnLoc = CameraLocation + (RandomDirection * SpawnRange);
	SpawnLoc.Z = PlayerCharacter->GetActorLocation().Z;
	return SpawnLoc;
}

// Called every frame
void AEnemy_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
