// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_HealthBar.generated.h"


UCLASS()
class TSSPROTOTYPE_API UWidget_HealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HealthBar")
	void UpdateHealthBar(float Health, float MaxHealth);

protected:
	UPROPERTY(meta =(BindWidget))
	class UProgressBar* HealthBar;
	
};
