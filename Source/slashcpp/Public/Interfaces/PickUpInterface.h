// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickUpInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASHCPP_API IPickUpInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetOverlapingItem(class AItem* Item);
	virtual void AddSouls(class ASoul* soul);
	virtual void AddGold(class ATreasure* Treasure);


};
