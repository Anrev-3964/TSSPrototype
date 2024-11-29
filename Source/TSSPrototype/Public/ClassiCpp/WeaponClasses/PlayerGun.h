// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate;

	FTimerHandle FireTimer;
};
