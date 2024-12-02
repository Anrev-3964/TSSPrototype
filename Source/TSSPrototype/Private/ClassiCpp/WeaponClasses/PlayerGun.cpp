// Fill out your copyright notice in the Description page of Project Settings.

#include "ClassiCpp/WeaponClasses/PlayerGun.h"


#include "Engine/World.h"
#include "TimerManager.h"
#include "ClassiCpp/WeaponClasses/BulletTypeStandard.h"
#include "GameFramework/Actor.h"



// Sets default values
APlayerGun::APlayerGun()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(RootComponent);
	FireRate = 0.3f;
	SpreadShotRange = 0.5f;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void APlayerGun::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerGun::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire"));
	if (!GetWorld() || !BulletClass) return;

	// Trace parameters
	FVector Start = MuzzleLocation->GetComponentLocation(); // Start of the trace (e.g., gun muzzle)
	FVector ForwardVector = GetActorForwardVector(); // Direction the gun is pointing
	//ForwardVector.X= FMath::RandRange(ForwardVector.X - 5, ForwardVector.X + 5);
	ForwardVector.Y= FMath::RandRange(ForwardVector.Y - SpreadShotRange, ForwardVector.Y + SpreadShotRange);
	FVector End = Start + (ForwardVector * 10000.0f); // End of the trace (10,000 units away)

	FHitResult HitResult; // Store trace results
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this); // Ignore the gun itself
	TraceParams.AddIgnoredActor(GetOwner()); // Ignore the owning actor (e.g., player character)

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,                   // Result of the trace
		Start,                       // Start of the trace
		End,                         // End of the trace
		ECC_Visibility,              // Collision channel
		TraceParams                  // Additional parameters
	);

	// Debug visualization
	FColor LineColor = bHit ? FColor::Red : FColor::Purple;
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 1.0f);

	
	FRotator Rotator = GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	BulletType = GetWorld()->SpawnActor<ABulletTypeStandard>(BulletClass, Start, Rotator, SpawnParams);
	if (BulletType)
	{
		BulletType->SetVelocity(ForwardVector);
	}
}

void APlayerGun::StartFiring()
{
	GetWorldTimerManager().SetTimer(FireTimer, this, &APlayerGun::Fire, FireRate, true);
}

void APlayerGun::StopFiring()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}


// Called every frame
void APlayerGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
