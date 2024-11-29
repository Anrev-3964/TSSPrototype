// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/WeaponClasses/PlayerGun.h"

// Sets default values
APlayerGun::APlayerGun()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	FireRate = 0.3f;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerGun::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire"));
}

// Called when the game starts or when spawned
void APlayerGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerGun::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerGun::Fire);
}

