// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakebleActor.generated.h"

class UGeometryCollectionComponent;
UCLASS()
class SLASHCPP_API ABreakebleActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	
	ABreakebleActor();

	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* CapsuleColition;
	
private:

	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>> ItemToSpawn;

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	float SpawnOffsetZ = 50.f;

};
