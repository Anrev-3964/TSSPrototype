// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ClassiCpp/Utilities/Enum_Elements.h"
#include "BulletTypeStandard.generated.h"


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInterface*> Materials;

	
	void BulletElement(AActor* Bullet);
	
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float Damage;
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	float FireRate;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Properties")
	float GetSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Properties")
	float GetDamage() const;
	UFUNCTION(BlueprintCallable, Category = "Properties")
	EDamageType GetDamageType() const;
	UFUNCTION(BlueprintCallable, Category = "Properties")
	void SetSpeed(float NewSpeed);
	UFUNCTION(BlueprintCallable, Category = "Properties")
	void SetDamage(float NewDamage);
	UFUNCTION(BlueprintCallable, Category = "Properties")
	void SetDamageType();
	
	void SetVelocity(const FVector& Velocity);
	
};



