// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	STANDARD UMETA(DisplayName = "Standard Damage"),
	FIRE UMETA(DisplayName = "Fire Damage"),
	COLD UMETA(DisplayName = "Cold Damage"),
};

class TSSPROTOTYPE_API Enum_Elements
{
public:
	Enum_Elements();
	~Enum_Elements();
};
