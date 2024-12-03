// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/EnemyClasses/FollowingEnemies.h"

// Sets default values
AFollowingEnemies::AFollowingEnemies()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFollowingEnemies::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFollowingEnemies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFollowingEnemies::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AFollowingEnemies::GetBehaviorTree() const
{
	return BehaviorTree;
}

