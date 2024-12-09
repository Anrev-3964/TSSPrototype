// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Win.generated.h"

/**
 * 
 */
UCLASS()
class TSSPROTOTYPE_API UWidget_Win : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Override NativeConstruct to initialize the widget
	virtual void NativeConstruct() override;

private:
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void RestartGame();
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void QuitGame();

	// Reference to buttons (set in the Blueprint)
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
};
