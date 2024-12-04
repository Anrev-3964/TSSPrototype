// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/WeaponClasses/BulletTypeStandard.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABulletTypeStandard::ABulletTypeStandard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	if (!Mesh) return;
	RootComponent = Mesh;
	
	Mesh->SetEnableGravity(false);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Mesh->SetGenerateOverlapEvents(true); //all this block of code ensures that the mesh doesn't respond to gravity,
				//doesn't have physics and an active collider and that acts as a trigger when overlapping a pawn
	Mesh->IgnoreActorWhenMoving(this, true); //This is done so the bullets don't get stuck on each other
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	if (!ProjectileMovement) return;
	Speed = ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	Damage = 5.0f;
	FireRate = 0.3f;
}

// Called when the game starts or when spawned
void ABulletTypeStandard::BeginPlay()
{
	Super::BeginPlay();
	BulletElement(this);
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
		if (Materials.Num() > 0 && Materials[0])  // Check if the array has materials
		{
			Mesh->SetMaterial(0, Materials[0]);  // Apply material to slot 0
		}
		break;
	case EDamageType::COLD:
		{
			Tags.Add(FName("Cold"));
			float ColdDamage = Damage/2;
			float ColdSpeed = Speed*3;
			float ColdFireRate = FireRate*2;
			if (Materials.Num() > 0 && Materials[1])
			{
				Mesh->SetMaterial(0, Materials[1]);
			}
			SetDamage(ColdDamage);
			SetSpeed(ColdSpeed);
			//SetFireRate(ColdFireRate);
		}
		break;
	case EDamageType::FIRE:
		{
			Tags.Add(FName("Fire"));
			float FireDamage = Damage*3;
			float FireSpeed = Speed/2;
			float FireFireRate = FireRate/2;
			if (Materials.Num() > 0 && Materials[2])
			{
				Mesh->SetMaterial(0, Materials[2]);
			}
			SetDamage(FireDamage);
			SetSpeed(FireSpeed);
			//SetFireRate(FireFireRate);
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
	if (NewDamage >=0)
	{
		Damage = NewDamage;
	}
}

void ABulletTypeStandard::SetSpeed(float NewSpeed)
{
	if (NewSpeed >=0)
	{
		Speed = NewSpeed;
	}
}

void ABulletTypeStandard::SetDamageType(float NewFireRate)
{
	if (NewFireRate >=0)
	{
		FireRate = NewFireRate;
	}
}

void ABulletTypeStandard::SetVelocity(const FVector& Velocity)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = Velocity * Speed;
	}
}

void ABulletTypeStandard::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !OtherActor->IsA(ABulletTypeStandard::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("Bullet overlapped with: %s"), *OtherActor->GetName());
		Destroy(); // Destroy or handle overlap appropriately
	}
}