// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/EnemyClasses/Enemy_Spawner.h"

// Sets default values
AEnemy_Spawner::AEnemy_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnRate = 1.0f;
	SpawnCount = 10;
	SpawnedEnemies = 0;
	MaxEnemies = 100;
}

// Called when the game starts or when spawned
void AEnemy_Spawner::BeginPlay()
{
	Super::BeginPlay();
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
	UE_LOG(LogTemp, Warning, TEXT("LOGIC ACTIVATED"));
	int Index = 0;
	if (EnemyToSpawn.Num() > 0 && EnemyToSpawn[Index])
	{
		FRotator Rotator = GetActorRotation();
		FVector Location = GetActorLocation();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		if (SpawnCount > 0)
		{
			for (int i = 0; i < SpawnCount; i++)
			{
				ACharacter* SpawnedEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyToSpawn[Index], Location, Rotator, SpawnParameters);
				if (SpawnedEnemy)
				{
					UE_LOG(LogTemp, Log, TEXT("Spawned Enemy: %s"), *SpawnedEnemy->GetName());
					SpawnedEnemies++;
				}
			}
		}
		else
		{
			ACharacter* SpawnedEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyToSpawn[Index], Location, Rotator, SpawnParameters);
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

// Called every frame
void AEnemy_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

