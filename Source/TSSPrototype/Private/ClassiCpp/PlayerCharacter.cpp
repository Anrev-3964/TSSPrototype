// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/PlayerCharacter.h"

#include "NaniteSceneProxy.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h" //without this I can't use the capsule
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter() 
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 500.0f; //how long I want this arm to be
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->SetRelativeRotation(FRotator(70.0f, 0.0f, 0.0f));
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false;
	
	PlayerCharacterSkeletalMesh = GetMesh();
	if (PlayerCharacterSkeletalMesh)
	{
		PlayerCharacterSkeletalMesh->SetupAttachment(GetCapsuleComponent()); // Attach mesh to capsule
		PlayerCharacterSkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		PlayerCharacterSkeletalMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	
	//Set a default skeletal mesh
	else if (PlayerCharacterSkeletalMesh == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh/Game/Models/SkeletalMeshes/PlayerCharacter/SK_PlayerCharacter"));
		if (MeshAsset.Succeeded())
		{
			PlayerCharacterSkeletalMesh->SetSkeletalMesh(MeshAsset.Object);
		}
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController()); 
	
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
	} //enable cursor view
	
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

void APlayerCharacter::Turn()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && PlayerCharacterSkeletalMesh)
	{
		FVector2D ScreenLocation;
		PlayerController->GetMousePosition(ScreenLocation.X, ScreenLocation.Y);

		FVector WorldLocation, WorldDirection;
		if (PlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection))
		{
			FVector TraceStart;
			FVector TraceEnd = TraceStart + (WorldLocation * 10000.0f);
			FHitResult HitTrace;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			if (GetWorld()->LineTraceSingleByChannel(HitTrace, TraceStart, TraceEnd,ECC_Visibility, Params))
			{
				FVector Impact = HitTrace.ImpactPoint;
				FVector Direction = Impact - GetActorLocation();
				Direction.Z = 0.0f;
				FRotator NewActorRotation =Direction.Rotation();

				PlayerCharacterSkeletalMesh->SetWorldRotation(NewActorRotation);
			}
		}
	}
	/*if (Controller != nullptr && Value != 0.0f)
	{
		
		FRotator CurrentRotation = PlayerCharacterSkeletalMesh->GetRelativeRotation();
		CurrentRotation.Yaw += Value; //Yaw added based on mouse input
		PlayerCharacterSkeletalMesh->SetRelativeRotation(CurrentRotation);
	}*/
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Turn();
	
	FVector StartLocation = GetCapsuleComponent()->GetComponentLocation(); //Get the location of the capsule bound to the player character
	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float TraceDistance = 100.0f; //The range of the downward raycast
	FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, CapsuleHalfHeight + TraceDistance);
	FHitResult Hit;
	
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility))
	{
		FVector NewLocation = Hit.ImpactPoint + FVector(0, 0, CapsuleHalfHeight);
		SetActorLocation(NewLocation, true);
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

