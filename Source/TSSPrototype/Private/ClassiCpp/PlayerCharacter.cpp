// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h" //without this i can't use the capsule

// Sets default values
APlayerCharacter::APlayerCharacter() 
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	
	PlayerCharacterSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerCharacterSkeletalMesh"));
	if (PlayerCharacterSkeletalMesh && Capsule)
	{
		PlayerCharacterSkeletalMesh->SetupAttachment(Capsule);

		//Mesh's position relative to the capsule i called
		PlayerCharacterSkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f)); // Adjust to fit character model
		PlayerCharacterSkeletalMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	
	// Optional: Set a default skeletal mesh (ensure you have a mesh in your assets)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/PathToYourMesh.YourMesh'"));
	if (MeshAsset.Succeeded())
	{
		PlayerCharacterSkeletalMesh->SetSkeletalMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		//Forward movement
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		//Right movement
		AddMovementInput(GetActorRightVector(), Value);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

