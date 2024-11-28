// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/PlayerCharacter.h"

#include "MaterialHLSLTree.h"
#include "NaniteSceneProxy.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h" //without this I can't use the capsule
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
		PlayerCharacterSkeletalMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
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
	//note that the line 58-59 is seen pretty common on the internet to find the cursor porjection
	//onto the internet, however, they do not give me the right vector.
	//FVector CursorWorldLocation, CursorWorldDirection;
	//GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);
	//=====================================================================//

	//gets location of the actor in the world
	FVector CurrLoc = PlayerCharacterSkeletalMesh->GetComponentLocation();

	// the right method of getting cursor location!!! note: i used the exact same method in the Epic Top Down Blueprint.
	FHitResult hitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true,hitResult);
	FVector HitLoc = hitResult.Location;

	//geting the original rotation of the acter;
	FRotator NewRot = PlayerCharacterSkeletalMesh->GetComponentRotation();
	//Using atan2 function to get the degree between our character location and cursor location
	// aka how much we want the character to rotate
	FVector RayDirection = (HitLoc - CurrLoc).GetSafeNormal();
	float newYaw = (RayDirection).Rotation().Yaw;
	//Using that degree as the Yaw(rotating around Z axis) of our Frotator
	NewRot.Yaw = newYaw;

	//in the end, we set it;
	PlayerCharacterSkeletalMesh->SetWorldRotation(NewRot);


	
		/*FVector TraceStart;
			FVector TraceEnd = TraceStart + (WorldLocation * 10000.0f);
			FHitResult HitTrace;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			if (GetWorld()->LineTraceSingleByChannel(HitTrace, TraceStart, TraceEnd,ECC_Visibility, Params))
			{
				FVector Impact = HitTrace.ImpactPoint;
				FVector Direction = Impact - GetActorLocation();
				Direction.Z = 0.0f;
				FRotator NewActorRotation = Direction.Rotation();

				PlayerCharacterSkeletalMesh->SetWorldRotation(NewActorRotation);
			}
		}*/
	
	/*if (Controller != nullptr && Value != 0.0f)
	{
		
		FRotator CurrentRotation = PlayerCharacterSkeletalMesh->GetRelativeRotation();
		CurrentRotation.Yaw += Value; //Yaw added based on mouse input
		PlayerCharacterSkeletalMesh->SetRelativeRotation(CurrentRotation);
	}*/
}

float APlayerCharacter::InterpolateRotation(float CurrentAngle, float TargetAngle, float Speed, float DeltaTime)
{
	float DeltaAngle = FMath::FindDeltaAngleDegrees(CurrentAngle, TargetAngle);

	float Step = Speed * DeltaTime;
	float ClampedDeltaAngle = FMath::Clamp(DeltaAngle, -Step, Step);

	return CurrentAngle + ClampedDeltaAngle;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Turn();

	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
	
	/*if (PlayerController && GetCapsuleComponent())
	{
		FVector2D ScreenLocation;
		PlayerController->GetMousePosition(ScreenLocation.X, ScreenLocation.Y);
		
		FVector CapsuleLocation = GetCapsuleComponent()->GetComponentLocation(); // Use CapsuleComponent location

		// Deproject the mouse position to world space
		FVector WorldLocation, WorldDirection;
		if (PlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection))
		{
			// Keep the direction flat (we're working in a top-down view, so ignore the Z-axis)
			WorldDirection.Z = 0;
			WorldDirection.Normalize();

			// Calculate the direction from the capsule to the mouse position in world space
			FVector DirectionToMouse = WorldLocation - CapsuleLocation;
			DirectionToMouse.Z = 0;  // Ensure we're working in a 2D plane (top-down view)
			DirectionToMouse.Normalize();

			// Get the current rotation of the capsule and the target rotation
			FRotator TargetRotation = DirectionToMouse.Rotation();
			FRotator CurrentRotation = GetCapsuleComponent()->GetComponentRotation();

			// Smoothly interpolate the rotation
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

			// Apply the new rotation to the capsule component (this affects both the capsule and its attached mesh)
			GetCapsuleComponent()->SetWorldRotation(NewRotation);

			// Debugging: Draw the direction of the capsule's forward vector
			FVector EndLocation = CapsuleLocation + (GetCapsuleComponent()->GetForwardVector() * 200.0f);
			DrawDebugLine(GetWorld(), CapsuleLocation, EndLocation, FColor::Green, false, -1.0f, 0, 5.0f);
		}
	}*/
	

	//Code to attach the character to the ground
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

