// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/UI/Widget_GameOver.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/Button.h"

void UWidget_GameOver::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize any custom functionality here (e.g., bind buttons to functions)
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UWidget_GameOver::RestartGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UWidget_GameOver::QuitGame);
	}
}
// Called when the "Restart" button is clicked
void UWidget_GameOver::RestartGame()
{
	// Unpause the game and restart the level
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);  // Reload the current level

	// Optionally, unpause the game (if it was paused)
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

// Called when the "Quit" button is clicked
void UWidget_GameOver::QuitGame()
{
	// Quit the game
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World ? World->GetFirstPlayerController() : nullptr;
	
	if (PlayerController)
	{
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
	}
}