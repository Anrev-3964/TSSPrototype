// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/UI/Widget_HealthBar.h"

#include "Components/ProgressBar.h"

void UWidget_HealthBar::UpdateHealthBar(float Health, float MaxHealth)
{
	if (HealthBar)
	{
		float FillBar= FMath::Clamp(Health / MaxHealth, 0.0f, 1.0f);
		HealthBar->SetPercent(FillBar);
	}
}
