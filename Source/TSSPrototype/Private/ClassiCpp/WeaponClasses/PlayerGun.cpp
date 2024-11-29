// Fill out your copyright notice in the Description page of Project Settings.

#include "ClassiCpp/WeaponClasses/PlayerGun.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"


// Sets default values
APlayerGun::APlayerGun()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	
	FireRate = 0.3f;
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
}

// Called every frame
void APlayerGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
