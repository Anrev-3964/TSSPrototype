// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/WeaponClasses/BulletTypeStandard.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABulletTypeStandard::ABulletTypeStandard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	RootComponent = Mesh;
	
	Damage = 5.0f;
	Speed = 2.0f;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	
}

// Called when the game starts or when spawned
void ABulletTypeStandard::BeginPlay()
{
	Super::BeginPlay();
	
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
		break;
	case EDamageType::COLD:
		{
			Tags.Add(FName("Cold"));
			float ColdDamage = Damage/2;
			float ColdSpeed = Speed*3;
			SetDamage(ColdDamage);
			SetSpeed(ColdSpeed);
		}
		break;
	case EDamageType::FIRE:
		{
			Tags.Add(FName("Fire"));
			float FireDamage = Damage*3;
			float FireSpeed = Speed/2;
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

void ABulletTypeStandard::SetVelocity(const FVector& Velocity)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = Velocity * Speed;
	}
}
