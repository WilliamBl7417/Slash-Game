// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Interfaces/PickUpInterface.h"
#include "slashcpp/DebugMacros.h"

ATreasure::ATreasure()
{
	PrimaryActorTick.bCanEverTick = true;
	CanIChangeFRotation = false;
}

void ATreasure::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterace = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterace)
	{
		PickUpInterace->AddGold(this);
		SpawnPickUpSound();
		Destroy();
	}
}
