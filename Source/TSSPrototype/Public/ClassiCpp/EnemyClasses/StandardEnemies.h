// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ClassiCpp/PlayerCharacter.h"
#include "ClassiCpp/Utilities/Enum_Elements.h"
#include "StandardEnemies.generated.h"

UCLASS()
class TSSPROTOTYPE_API AStandardEnemies : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStandardEnemies();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta=(AllowPrivateAccess = "true"))
	APlayerCharacter* Player;

	FVector RandomTargetOffset;
	float ChangeTargetTime;
	float OscillationPhase; // Unique phase offset for sine wave
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float DamageDealt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AActor>> Drops;

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool bCanDealDamage = true;  // Flag to track if damage can be applied
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float DamageCooldown = 2.0f; // Time in seconds between damage applications
	
	void FollowPlayer();
	void DropPickup();
	void EnableDamage();

	FTimerHandle DamageCooldownTimerHandle;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetHealth(float DamageTaken);
	
};
