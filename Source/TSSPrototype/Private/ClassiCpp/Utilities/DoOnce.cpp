// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassiCpp/Utilities/DoOnce.h"

// Constructor
DoOnce::DoOnce()
	: bHasExecuted(false) {}

// Resets the state to allow the logic to execute again
void DoOnce::Reset()
{
	bHasExecuted = false;
}

