// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/CustomPlayerController.h"

#include "ClassiCpp/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

ACustomPlayerController::ACustomPlayerController()
{
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ACustomPlayerController BeginPlay called"));
	if (HealthBarWidgetClass)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
		if (PlayerCharacter)
		{
			float CurrentHealth = PlayerCharacter->GetHealth();
			float MaxHealth = PlayerCharacter->GetMaxHealth();
			//Gets both the max value for Health and the temporary value, in order to calculate the bar's filling percentage

			HealthBarWidget = CreateWidget<UWidget_HealthBar>(this, HealthBarWidgetClass);
			if (HealthBarWidget)
			{
				HealthBarWidget->AddToViewport();
				UE_LOG(LogTemp, Warning, TEXT("Initial Health: %f / %f"), CurrentHealth, MaxHealth);
				UIHealth(CurrentHealth, MaxHealth);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("HealthBarWidget could not be created!"));
			}
		}
	}
	else
	{
		{
			UE_LOG(LogTemp, Error, TEXT("HealthBarWidget could not be created!"));
		}
	}
}

void ACustomPlayerController::UIManager()
{
}

void ACustomPlayerController::UIHealth(float CurrentHealth, float MaxHealth)
//Used to manage the HealthBar UI and the GameOver UI here, instead of putting it in the PlayerCharacter
{
	if (HealthBarWidget)
	{
		HealthBarWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("COULD NOT CREATE HEALTH BAR"));
	}

	if (CurrentHealth <= 0 && !GameOverWidget) // Ensure the GameOver is triggered only once
	{
		if (!GameOverWidget)
		{
			// Create and add the widget to the viewport
			GameOverWidget = CreateWidget<UWidget_GameOver>(this, GameOverWidgetClass);
			if (GameOverWidget)
			{
				GameOverWidget->AddToViewport();

				// Pause the game
				UGameplayStatics::SetGamePaused(GetWorld(), true);

				// Show mouse cursor for the widget
				this->bShowMouseCursor = true;
				this->SetInputMode(FInputModeUIOnly());
				UE_LOG(LogTemp, Error, TEXT("ENTERED THIRD IF"));
			}
			UE_LOG(LogTemp, Error, TEXT("ENTERED SECOND IF"));
		}
		UE_LOG(LogTemp, Error, TEXT("ENTERED FIRST IF"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), CurrentHealth);
}

void ACustomPlayerController::UIWin(bool WinCondition)
{
	if (WinCondition && !WinWidget) //Ensure it's called only once
	{
		WinWidget = CreateWidget<UWidget_Win>(this, WinWidgetClass);
		if (WinWidget)
		{
			WinWidget->AddToViewport();

			UGameplayStatics::SetGamePaused(GetWorld(), true);

			this->bShowMouseCursor = true;
			this->SetInputMode(FInputModeUIOnly());
		}
	}
}
