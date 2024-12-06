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
		Capsule->SetNotifyRigidBodyCollision(true);
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Capsule->SetCollisionObjectType(ECC_GameTraceChannel2);
		Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		Capsule->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
		Capsule->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

		Capsule->OnComponentBeginOverlap.AddDynamic(this, &AStandardEnemies::OnCapsuleBeginOverlap);
		Capsule->OnComponentEndOverlap.AddDynamic(this, &AStandardEnemies::OnCapsuleEndOverlap);
		Capsule->OnComponentHit.AddDynamic(this, &AStandardEnemies::OnHit);
	}

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Speed = 650.0f;
	Health = 100.0f;
	DamageDealt = 10.0f;

	RandomTargetOffset = FVector::ZeroVector;
	ChangeTargetTime = 0.0f;
	OscillationPhase = FMath::FRandRange(0.0f, 2.0f * PI); // Randomize initial phase


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
	this->SetActorRotation(FRotator(0,0,0));
	FollowPlayer();
	if (Health <= 0)
	{
		DropPickup();
		Destroy();
	}
}

void AStandardEnemies::SetHealth(float DamageTaken)
{
	if (DamageTaken)
	{
		Health -= DamageTaken;
		UE_LOG(LogTemp, Display, TEXT("New Health: %f"), Health);
	}
}

void AStandardEnemies::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this) // Ensure it doesn't overlap itself
	{
		
		if (OtherActor->IsA(ABulletTypeStandard::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy collided with: %s"), *OtherActor->GetName());
			ABulletTypeStandard* Bullet = Cast<ABulletTypeStandard>(OtherActor);
			float DamageTaken = Bullet->GetDamage();
			SetHealth(DamageTaken);
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

void AStandardEnemies::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && bCanDealDamage)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy COLLIDED with: %s"), *OtherActor->GetName());

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if (PlayerCharacter)
		{
			PlayerCharacter->SetHealth(DamageDealt);
			bCanDealDamage = false; // Disable damage until cooldown is finished

			// Set a timer to re-enable damage after the cooldown
			GetWorld()->GetTimerManager().SetTimer(DamageCooldownTimerHandle, this, &AStandardEnemies::EnableDamage, DamageCooldown, false);
		}
	}
} //Problem: on multiple enemies the damage is applied multiple times

void AStandardEnemies::FollowPlayer()
{
	if (Player)
	{
		FVector PlayerLocation = Player->GetCapsuleComponent()->GetComponentLocation();
	    FVector EnemyLocation = GetActorLocation();

	    // Calculate base direction towards the player
	    FVector DirectionToPlayer = (PlayerLocation - EnemyLocation).GetSafeNormal();

	    // Add orbital movement
	    FVector PerpendicularDirection = FVector(-DirectionToPlayer.Y, DirectionToPlayer.X, 0.0f); // Perpendicular in 2D
	    float OrbitStrength = 0.5f; // Blend factor: 0.0 = direct, 1.0 = full orbit

	    // Add randomness to the target position near the player
	    if (ChangeTargetTime <= 0.0f)
	    {
	        float RandomRadius = 50.0f; // Maximum distance from player
	        RandomTargetOffset = FVector(
	            FMath::FRandRange(-RandomRadius, RandomRadius),
	            FMath::FRandRange(-RandomRadius, RandomRadius),
	            0.0f); // No vertical offset for top-down game
	        ChangeTargetTime = FMath::FRandRange(1.0f, 2.0f); // Change interval
	    }
	    else
	    {
	        ChangeTargetTime -= GetWorld()->DeltaTimeSeconds;
	    }

	    // Random target point around the player
	    FVector TargetLocation = PlayerLocation + RandomTargetOffset;

	    // Direction towards the random target
	    FVector DirectionToTarget = (TargetLocation - EnemyLocation).GetSafeNormal();

	    // Blend between direct movement and orbital movement
	    FVector BlendedDirection = (1.0f - OrbitStrength) * DirectionToPlayer + OrbitStrength * PerpendicularDirection;

	    // Add sine wave oscillation for erratic behavior
	    float Time = GetWorld()->GetTimeSeconds();
	    float SineOffsetStrength = 200.0f;
	    FVector SineOffset = FVector(
	        FMath::Sin(Time * 2.0f + OscillationPhase) * SineOffsetStrength,
	        FMath::Cos(Time * 2.0f + OscillationPhase) * SineOffsetStrength,
	        0.0f);

	    // Final direction combining all effects
	    FVector FinalDirection = DirectionToTarget + BlendedDirection + SineOffset.GetSafeNormal();
	    FinalDirection = FinalDirection.GetSafeNormal();

	    // Movement delta
	    FVector MovementDelta = FinalDirection * Speed * GetWorld()->DeltaTimeSeconds;

	    // Move the actor, respecting collision
	    AddActorWorldOffset(MovementDelta, true);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player is not valid"));
	}
}

void AStandardEnemies::DropPickup()
{
	float DropChance = 25.0f;
	float DropValue = FMath::RandRange(0.0f, 100.0f); //simulating a percentage of dropping an item
	
	if (DropChance >= DropValue)
	{
		if (Drops.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, Drops.Num() - 1);
			TSubclassOf<AActor> SelectedDrop = Drops[RandomIndex];

			FVector SpawnLocation = GetActorLocation(); 
			FRotator SpawnRotation = FRotator::ZeroRotator;

			GetWorld()->SpawnActor<AActor>(SelectedDrop, SpawnLocation, SpawnRotation); //spawns the drop at the enemy location
			UE_LOG(LogTemp, Log, TEXT("Dropped item: %s"), *SelectedDrop->GetName());
		} //drop logic
	}
}

void AStandardEnemies::EnableDamage()
{
	bCanDealDamage = true;
}


