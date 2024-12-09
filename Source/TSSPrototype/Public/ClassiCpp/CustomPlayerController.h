// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/Widget_GameOver.h"
#include "UI/Widget_HealthBar.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TSSPROTOTYPE_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACustomPlayerController();

protected:
	virtual void BeginPlay() override;
	void UIManager();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidget_GameOver* GameOverWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UWidget_HealthBar* HealthBarWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	void UIHealth(float CurrentHealth, float MaxHealth);
};
