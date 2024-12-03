// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "ClassiCpp/PlayerCharacter.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UBehaviorTree* GetBehaviorTree() const;

};
