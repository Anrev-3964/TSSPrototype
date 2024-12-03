// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/EnemyClasses/StandardEnemies.h"

#include "MaterialHLSLTree.h"
#include "ClassiCpp/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AStandardEnemies::AStandardEnemies()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetCapsuleComponent();
	

	Speed = 500.0f;

}

void AStandardEnemies::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStandardEnemies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBehaviorTree* AStandardEnemies::GetBehaviorTree() const
{
	return BehaviorTree;
}

