// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/EnemyClasses/Pickup.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (!Mesh) return;
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	Index = FMath::RandRange(0, Materials.Num() - 1);
	switch (Index)
	{
	case 0:
		Element = EDamageType::COLD;
		Tags.Add(FName("Cold"));
		break;
	case 1:
		Element = EDamageType::FIRE;
		Tags.Add(FName("Fire"));
		break;
	default:
		Element = EDamageType::STANDARD; // Default or fallback value
		break;
	}
	PickupElement(this);
}

void APickup::PickupElement(AActor* Pickup)
{
	if (!Mesh || Materials.Num() <= Index || !Materials[Index])
	{
		UE_LOG(LogTemp, Error, TEXT("Material not valid or out of range: Index=%d, Materials.Num()=%d"), Index, Materials.Num());
		return;
	}

	switch (Element)
	{
	case EDamageType::FIRE:
	case EDamageType::COLD:
		UE_LOG(LogTemp, Error, TEXT("Applying material: %s"), *Materials[Index]->GetName());
		Mesh->SetMaterial(0, Materials[Index]);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("No matching element for material assignment."));
		break;
	}
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

