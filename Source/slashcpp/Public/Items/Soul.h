// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Soul.generated.h"

/**
 * 
 */

UCLASS()
class SLASHCPP_API ASoul : public AItem
{
	GENERATED_BODY()
public:
	ASoul();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay()override;

	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

		UPROPERTY(EditAnywhere, Category = "PickUp Properties|Soul")
		int32 Souls;

		double DesiredZ = 0.f;
		UPROPERTY(EditAnywhere, Category = "PickUp Properties|Soul")
		float DriftSpeed = 10.f;

public:
	FORCEINLINE int32 GetSouls()const { return Souls; }
	FORCEINLINE void SetSouls(int32 NumberofSouls) { Souls = NumberofSouls; }
};