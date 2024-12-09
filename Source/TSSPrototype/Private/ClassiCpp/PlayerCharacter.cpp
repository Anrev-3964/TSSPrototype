// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "ClassiCpp/CustomPlayerController.h"
#include "ClassiCpp/EnemyClasses/StandardEnemies.h"
#include "ClassiCpp/WeaponClasses/PlayerGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h" //without this I can't use the capsule
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;

	//this root component is used so my camera doesn't rotate with the capsule while pointing at the cursor
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->Mobility = EComponentMobility::Movable;
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetupAttachment(RootComponent);

	if (CapsuleComp)
	{
		CapsuleComp->SetGenerateOverlapEvents(true);
		CapsuleComp->SetEnableGravity(false);
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CapsuleComp->SetCollisionObjectType(ECC_Pawn);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
		CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
		CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
		CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		CapsuleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f; //how long I want this arm to be
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->SetRelativeRotation(FRotator(70.0f, 0.0f, 0.0f));

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);


	PlayerCharacterSkeletalMesh = GetMesh();
	if (PlayerCharacterSkeletalMesh)
	{
		PlayerCharacterSkeletalMesh->SetupAttachment(CapsuleComp); // Attach mesh to capsule
		PlayerCharacterSkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		PlayerCharacterSkeletalMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}

	//Set a default skeletal mesh
	else if (PlayerCharacterSkeletalMesh == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(
			TEXT("SkeletalMesh/Game/Models/SkeletalMeshes/PlayerCharacter/SK_PlayerCharacter"));
		if (MeshAsset.Succeeded())
		{
			PlayerCharacterSkeletalMesh->SetSkeletalMesh(MeshAsset.Object);
		}
	}
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ACustomPlayerController* PlayerController = Cast<ACustomPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Allows free mouse movement
		InputMode.SetHideCursorDuringCapture(false); // Cursor stays visible
		PlayerController->SetInputMode(InputMode);
	} //enables cursor view

	if (PlayerCamera)
	{
		PlayerCamera->bUsePawnControlRotation = false;
	}

	if (PlayerGunClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this; //the player is the owner of the weapon
		SpawnParams.Instigator = GetInstigator();

		EquippedGun = GetWorld()->SpawnActor<APlayerGun>(PlayerGunClass, FVector::ZeroVector,
		                                                 FRotator::ZeroRotator, SpawnParams);
		if (EquippedGun)
		{
			EquippedGun->AttachToComponent(GetCapsuleComponent(),
			                               FAttachmentTransformRules::SnapToTargetIncludingScale);
			UE_LOG(LogTemp, Warning, TEXT("WEAPON ASSIGNED"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO WEAPON ASSIGNED"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO GUN CLASS"));
	}
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

void APlayerCharacter::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("FireFromCharacter"));
	if (EquippedGun)
	{
		EquippedGun->StartFiring();
	}

	Turn();
	//This is needed since, if not present, the mouse gets locked in the fire position when the function is called
}

void APlayerCharacter::StopWeapon()
{
	if (EquippedGun)
	{
		EquippedGun->StopFiring();
	}
}

void APlayerCharacter::Turn()
{
	//gets location of the actor in the world
	FVector CurrLoc = GetCapsuleComponent()->GetComponentLocation();

	// the right method of getting cursor location!!! note: i used the exact same method in the Epic Top Down Blueprint.
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true, HitResult);
	FVector HitLoc = HitResult.Location;

	//geting the original rotation of the acter;
	FRotator NewRot = GetCapsuleComponent()->GetComponentRotation();
	//Using atan2 function to get the degree between our character location and cursor location
	// aka how much we want the character to rotate
	FVector RayDirection = (HitLoc - CurrLoc).GetSafeNormal();
	float NewYaw = (RayDirection).Rotation().Yaw;
	//Using that degree as the Yaw(rotating around Z axis) of our Frotator
	NewRot.Yaw = NewYaw;

	//in the end, we set it;
	GetCapsuleComponent()->SetWorldRotation(NewRot);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CapsuleX = GetCapsuleComponent()->GetRelativeLocation().X;
	float CapsuleY = GetCapsuleComponent()->GetRelativeLocation().Y;
	SpringArm->SetRelativeLocation(FVector(CapsuleX, CapsuleY, SpringArm->GetRelativeLocation().Z));
	//Now the SpringArm follows the capsule

	Turn();

	if (!GetCharacterMovement()->IsMovingOnGround())
	//This script normally interferes with friction, so it's only called when the character is not touching ground
	{
		//Code to attach the character to the ground
		FVector StartLocation = GetCapsuleComponent()->GetComponentLocation();
		//Get the location of the capsule bound to the player character
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
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::FireWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopWeapon);
}

float APlayerCharacter::GetHealth()
{
	return CurrentHealth;
}

float APlayerCharacter::GetMaxHealth()
{
	return MaxHealth;
}

void APlayerCharacter::SetHealth(float DamageTaken)
{
	CurrentHealth -= DamageTaken;
	ACustomPlayerController* PlayerController = Cast<ACustomPlayerController>(GetController());
	PlayerController->UIHealth(CurrentHealth, MaxHealth);
}
