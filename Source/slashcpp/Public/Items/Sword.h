// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Sword.generated.h"

class UBoxComponent;
class USceneComponent;

UCLASS()
class SLASHCPP_API ASword : public AItem

{
	GENERATED_BODY()

public:
	ASword();

	void Equip(USceneComponent* InParent, FName InSocketName,AActor* newOwner, APawn* newInstigator);
	void AttachMesToSocket(USceneComponent* InParent, const FName& InSocketName);
	UPROPERTY(VisibleAnywhere, Category = "Weapon Variables")
	UBoxComponent* WeaponBox;
	TArray<AActor*> IgnoreActors;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void ExecuteGetHit(FHitResult& BoxHit);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FielLocation);

private:

	void BoxTrace(FHitResult& BoxHit);
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtend = FVector(5.f);
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStar;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Variables")
	float Damage = 20;
public:
	FORCEINLINE	UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
