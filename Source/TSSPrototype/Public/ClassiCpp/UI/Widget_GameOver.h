// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_GameOver.generated.h"

/**
 * 
 */
UCLASS()
class TSSPROTOTYPE_API UWidget_GameOver : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void RestartGame();
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void QuitGame();
protected:
	// Override NativeConstruct to initialize the widget
	virtual void NativeConstruct() override;
};
