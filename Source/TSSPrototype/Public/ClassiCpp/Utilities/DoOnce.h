// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class TSSPROTOTYPE_API DoOnce
{
public:
	// Constructor
	DoOnce();

	// Executes the provided lambda only once
	template <typename FuncType>
	void Execute(FuncType&& Func)
	{
		if (!bHasExecuted)
		{
			Func();
			bHasExecuted = true;
		}
	}

	// Resets the state to allow the logic to execute again
	void Reset();

private:
	// Keeps track of whether the logic has been executed
	bool bHasExecuted;
};
