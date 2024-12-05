// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/UI/Widget_GameOver.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UWidget_GameOver::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize any custom functionality here (e.g., bind buttons to functions)
}
// Called when the "Restart" button is clicked
void UWidget_GameOver::RestartGame()
{
	// Unpause the game and restart the level
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()));  // Reload the current level

	// Optionally, unpause the game (if it was paused)
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

// Called when the "Quit" button is clicked
void UWidget_GameOver::QuitGame()
{
	// Unpause the game and load the main menu level
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));  // Replace "MainMenu" with your actual main menu level name

	// Optionally, unpause the game (if it was paused)
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}