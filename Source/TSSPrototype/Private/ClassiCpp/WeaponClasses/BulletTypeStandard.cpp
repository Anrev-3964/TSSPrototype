// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/WeaponClasses/BulletTypeStandard.h"

#include "ClassiCpp/EnemyClasses/StandardEnemies.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABulletTypeStandard::ABulletTypeStandard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	if (!Mesh)
	{
		return;
	}
	RootComponent = Mesh;

	if (Mesh)
	{
		Mesh->SetEnableGravity(false);
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Mesh->SetCollisionObjectType(ECC_GameTraceChannel1);
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
		Mesh->SetGenerateOverlapEvents(true); //all this block of code ensures that the mesh doesn't respond to gravity,
		//doesn't have physics and an active collider and that acts as a trigger when overlapping a pawn
		Mesh->IgnoreActorWhenMoving(this, true); //This is done so the bullets don't get stuck on each other
		Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABulletTypeStandard::OnOverlapBegin);
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	if (!ProjectileMovement)
	{
		return;
	}
	Speed = ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	Damage = 20.0f;
	BulletLifetime = 5.0f;
}

// Called when the game starts or when spawned
void ABulletTypeStandard::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(BulletEraserTimer, this, &ABulletTypeStandard::BulletEraser, BulletLifetime, false);
}

void ABulletTypeStandard::BulletEraser()
{
	GetWorldTimerManager().ClearTimer(BulletEraserTimer);
	this->Destroy(); //This function destroys the bullet after the specified lifetime
}

// Called every frame
void ABulletTypeStandard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABulletTypeStandard::BulletElement(AActor* Bullet)
{
	switch (DamageType)
	{
	case EDamageType::STANDARD:
		Tags.Add(FName("Standard"));
		if (Materials.Num() > 0 && Materials[0]) // Check if the array has materials
		{
			Mesh->SetMaterial(0, Materials[0]); // Apply material to slot 0
		}
		break;
	case EDamageType::COLD:
		{
			Tags.Add(FName("Cold"));
			float ColdDamage = Damage / 2;
			float ColdSpeed = Speed * 3;
			if (Materials.Num() > 0 && Materials[1])
			{
				Mesh->SetMaterial(0, Materials[1]);
			}
			SetDamage(ColdDamage);
			SetSpeed(ColdSpeed);
		}
		break;
	case EDamageType::FIRE:
		{
			Tags.Add(FName("Fire"));
			float FireDamage = Damage * 3;
			float FireSpeed = Speed / 2;
			if (Materials.Num() > 0 && Materials[2])
			{
				Mesh->SetMaterial(0, Materials[2]);
			}
			SetDamage(FireDamage);
			SetSpeed(FireSpeed);
		}
		break;
	default:
		break;
	}
}

float ABulletTypeStandard::GetDamage() const
{
	return Damage;
}

float ABulletTypeStandard::GetSpeed() const
{
	return Speed;
}

EDamageType ABulletTypeStandard::GetDamageType() const
{
	return DamageType;
}

void ABulletTypeStandard::SetDamage(float NewDamage)
{
	if (NewDamage >= 0)
	{
		Damage = NewDamage;
	}
}

void ABulletTypeStandard::SetSpeed(float NewSpeed)
{
	if (NewSpeed >= 0)
	{
		Speed = NewSpeed;

		// Update the current velocity to reflect the new speed
		if (ProjectileMovement)
		{
			ProjectileMovement->Velocity = ProjectileMovement->Velocity.GetSafeNormal() * Speed;
		}
	}
}

void ABulletTypeStandard::SetDamageType(EDamageType NewDamageType)
{
	DamageType = NewDamageType;
	BulletElement(this);
}

void ABulletTypeStandard::SetVelocity(const FVector& Velocity)
{
	if (ProjectileMovement)
	{
		FVector NormalizedVelocity = Velocity.GetSafeNormal();
		ProjectileMovement->Velocity = NormalizedVelocity * Speed;

		// Debug log
		UE_LOG(LogTemp, Warning, TEXT("Bullet Velocity: %s"), *ProjectileMovement->Velocity.ToString());
	}
}

void ABulletTypeStandard::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(AStandardEnemies::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("Bullet overlapped with: %s"), *OtherActor->GetName());
			this->Destroy(); // Destroy on overlap
		}
	}
}
