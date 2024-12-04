// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/EnemyClasses/StandardEnemies.h"

#include "ClassiCpp/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AStandardEnemies::AStandardEnemies()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetupAttachment(RootComponent);
	Speed = 500.0f;
}

void AStandardEnemies::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	Player = Cast<APlayerCharacter>(PlayerController->GetCharacter());
}

// Called every frame
void AStandardEnemies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FollowPlayer();

}

void AStandardEnemies::FollowPlayer()
{
	if (Player)
	{
		// Get the player's location
		FVector PlayerLocation = Player->GetCapsuleComponent()->GetComponentLocation();
        
		// Get the enemy's current location
		FVector EnemyLocation = GetActorLocation();

		// Calculate the direction vector from the enemy to the player
		FVector DirectionToPlayer = PlayerLocation - EnemyLocation;

		// Normalize the direction vector to avoid moving too fast (scale to unit length)
		DirectionToPlayer.Normalize();

		// Move the enemy in the direction of the player
		FVector MovementDelta = DirectionToPlayer * Speed * GetWorld()->DeltaTimeSeconds;

		// Add movement towards the player
		AddActorWorldOffset(MovementDelta); // This moves the actor smoothly
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player is not valid"));
	}
}



