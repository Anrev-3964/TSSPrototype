// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/UI/Widget_Win.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UWidget_Win::NativeConstruct()
{
	Super::NativeConstruct();

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UWidget_Win::RestartGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UWidget_Win::QuitGame);
	}
}

// Called when the "Restart" button is clicked
void UWidget_Win::RestartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false); // Reload the current level
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

// Called when the "Quit" button is clicked
void UWidget_Win::QuitGame()
{
	UWorld* World = GetWorld();

	APlayerController* PlayerController = World ? World->GetFirstPlayerController() : nullptr;
	if (PlayerController)
	{
		UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false); // Quit the game
	}
}
