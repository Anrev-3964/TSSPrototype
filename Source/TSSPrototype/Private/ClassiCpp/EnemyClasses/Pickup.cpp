// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/EnemyClasses/Pickup.h"

#include "ClassiCpp/PlayerCharacter.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (!Mesh) return;
	RootComponent = Mesh;
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel3);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnPickupOverlap);
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
	case EDamageType::COLD:
	case EDamageType::FIRE:
		Mesh->SetMaterial(0, Materials[Index]);
		break;
	default:
		break;
	}
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::OnPickupOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if (OtherActor && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		// Cast the overlapping actor to the player character
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

		if (Player && Player->EquippedGun)
		{
			// Directly modify the gun's elemental type
			Player->EquippedGun->SetBulletElement(Element);
			UE_LOG(LogTemp, Log, TEXT("Elemental type changed to %d"), static_cast<int32>(Element));

			// Destroy the pickup after it's used
			Destroy();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player or PlayerGun is null!"));
		}
	}*/
}


