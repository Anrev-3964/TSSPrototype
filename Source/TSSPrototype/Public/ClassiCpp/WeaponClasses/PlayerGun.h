// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletTypeStandard.h"
#include "GameFramework/Actor.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();
	
	void StartFiring();
	void StopFiring();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float SpreadShotRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Bullets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABulletTypeStandard> BulletClass;
	UPROPERTY(EditAnywhere, Category = "Bullets")
	ABulletTypeStandard* BulletType;

	FTimerHandle FireTimer;
};
