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
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetupAttachment(RootComponent); //?

	if (Capsule)
	{
		Capsule->SetGenerateOverlapEvents(true);
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Capsule->SetCollisionObjectType(ECC_Pawn);
		Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		Capsule->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

		Capsule->OnComponentBeginOverlap.AddDynamic(this, &AStandardEnemies::OnCapsuleBeginOverlap);
		Capsule->OnComponentEndOverlap.AddDynamic(this, &AStandardEnemies::OnCapsuleEndOverlap);
	}

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Speed = 500.0f;
	Health = 100.0f;

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

void AStandardEnemies::SetHealth(float DamageTaken)
{
	if (DamageTaken)
	{
		Health -= DamageTaken;
	}
}

void AStandardEnemies::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this) // Ensure it doesn't overlap itself
	{
		if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy collided with: %s"), *OtherActor->GetName());
		}
		if (OtherActor->IsA(ABulletTypeStandard::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy collided with: %s"), *OtherActor->GetName());
		}
	}
}

void AStandardEnemies::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy stopped overlapping with: %s"), *OtherActor->GetName());
		}
	}
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



