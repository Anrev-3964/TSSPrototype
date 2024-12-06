// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletTypeStandard.h"
#include "ClassiCpp/Utilities/Enum_Elements.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInterface.h"
#include "PlayerGun.generated.h"

UCLASS()
class TSSPROTOTYPE_API APlayerGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerGun();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float TempFireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float FireRate;
	bool bCanFire;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	void ResetFireBoolCooldown();
	void StartFiring();
	void StopFiring();
	void SetBulletElement(EDamageType NewDamageType);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleLocation;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SpreadShotRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Bullets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABulletTypeStandard> BulletClass;
	UPROPERTY(EditAnywhere, Category = "Bullets")
	ABulletTypeStandard* BulletType;
	UPROPERTY(EditAnywhere, Category = "Bullets")
	EDamageType BulletDamageType;
	
	
	FTimerHandle FireTimer;
	FTimerHandle OpenFireTimer;
};
