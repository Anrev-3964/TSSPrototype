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
	SpreadShotRange = 0.15f;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BulletDamageType = EDamageType::STANDARD;
	FireRate = 0.2f;
	bCanFire = true;
}


// Called when the game starts or when spawned
void APlayerGun::BeginPlay()
{
	Super::BeginPlay();

	TempFireRate = FireRate;
}

void APlayerGun::Fire()
{
	
	if (!bCanFire || !GetWorld() || !BulletClass)
	{
		return; // Do nothing if cooldown is active or prerequisites are not met
	}
	

	
		// Trace parameters
	FVector Start = MuzzleLocation->GetComponentLocation(); // Start of the trace (e.g., gun muzzle)
	FVector ForwardVector = GetActorForwardVector(); // Direction the gun is pointing
	{
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
	}

	
	FRotator Rotator = GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	BulletType = GetWorld()->SpawnActor<ABulletTypeStandard>(BulletClass, Start, Rotator, SpawnParams);
	if (BulletType)
	{
		BulletType->SetDamageType(BulletDamageType); // Update the bullet's type
		BulletType->SetVelocity(ForwardVector);      // Set its velocity
	}
	
	// Set cooldown
	/*bCanFire = false;
	GetWorldTimerManager().SetTimer(OpenFireTimer, this, &APlayerGun::ResetFireBoolCooldown, TempFireRate, false);*/
}

void APlayerGun::ResetFireBoolCooldown()
{
	bCanFire = true;
}

void APlayerGun::StartFiring()
{
	if (!BulletClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletClass is not assigned!"));
		return;
	}
	
	switch (BulletDamageType)
		{
		case EDamageType::STANDARD:
			{
				TempFireRate = FireRate;
				GetWorldTimerManager().SetTimer(FireTimer, this, &APlayerGun::Fire, TempFireRate, true);
			}
			break;
		case EDamageType::COLD:
			{
				TempFireRate = FireRate / 2;
				GetWorldTimerManager().SetTimer(FireTimer, this, &APlayerGun::Fire, TempFireRate, true);
			}
			break;
		case EDamageType::FIRE:
			{
				TempFireRate = FireRate * 2;
				GetWorldTimerManager().SetTimer(FireTimer, this, &APlayerGun::Fire, TempFireRate, true);
			}
			break;
		default:
			{
				UE_LOG(LogTemp, Warning, TEXT("UNKNOWN DAMAGE TYPE"));
			}
		}

	// Fire the first bullet immediately if allowed
	/*if (bCanFire)
	{
		Fire();
	}
	// Start periodic firing
	if (!GetWorldTimerManager().IsTimerActive(FireTimer))
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &APlayerGun::Fire, TempFireRate, true);
	}*/
	
}

void APlayerGun::StopFiring()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
	/*GetWorldTimerManager().ClearTimer(OpenFireTimer);
	bCanFire = true;*/
}

void APlayerGun::SetBulletElement(EDamageType NewDamageType)
{
	BulletDamageType = NewDamageType;
	StopFiring();
	StartFiring(); //this makes the Fire Rate change instantly when a new element is taken
	UE_LOG(LogTemp, Error, TEXT("FUNCTION ACTIVE"));
	
}


// Called every frame
void APlayerGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
