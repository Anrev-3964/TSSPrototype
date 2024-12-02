// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BulletTypeStandard.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	STANDARD UMETA(DisplayName = "Standard Damage"),
	FIRE UMETA(DisplayName = "Fire Damage"),
	COLD UMETA(DisplayName = "Cold Damage"),
};

UCLASS()

class TSSPROTOTYPE_API ABulletTypeStandard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletTypeStandard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	EDamageType DamageType {EDamageType::STANDARD};
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;
	
	void BulletElement(AActor* Bullet);
	
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float Damage;
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float Speed;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Properties")
	float GetSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Properties")
	float GetDamage() const;
	UFUNCTION(BlueprintCallable, Category = "Properties")
	void SetSpeed(float NewSpeed);
	UFUNCTION(BlueprintCallable, Category = "Properties")
	void SetDamage(float NewDamage);
	
	void SetVelocity(const FVector& Velocity);
	
};



