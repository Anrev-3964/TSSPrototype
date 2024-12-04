// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "ClassiCpp/PlayerCharacter.h"
#include "Enemy_Spawner.generated.h"

UCLASS()
class TSSPROTOTYPE_API AEnemy_Spawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_Spawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnLogic();
	UFUNCTION()
	void AttachToPlayer();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Instance", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ACharacter>> EnemyToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Instance", meta = (AllowPrivateAccess = "true"))
	FVector SpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Instance", meta = (AllowPrivateAccess = "true"))
	float SpawnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Instance", meta = (AllowPrivateAccess = "true"))
	int SpawnCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Instance", meta = (AllowPrivateAccess = "true"))
	int MaxEnemies;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn Instance", meta = (AllowPrivateAccess = "true"))
	int SpawnedEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	APlayerCharacter* PlayerCharacter;

	FTimerHandle SpawnTimer;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
