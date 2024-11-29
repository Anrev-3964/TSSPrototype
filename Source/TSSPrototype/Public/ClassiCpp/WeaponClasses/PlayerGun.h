// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerGun.generated.h"

UCLASS()
class TSSPROTOTYPE_API APlayerGun : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerGun();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate;

	FTimerHandle FireTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
