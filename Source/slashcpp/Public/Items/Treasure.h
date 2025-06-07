// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class SLASHCPP_API ATreasure : public AItem
{
	GENERATED_BODY()

public:

	ATreasure();

protected:

	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

private:

	UPROPERTY(EditAnywhere, Category = "PickUp Properties|Treasure")
	int32 Gold;

public:
	FORCEINLINE int32 GetGold()const { return Gold; }
};
