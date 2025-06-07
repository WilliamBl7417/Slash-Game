// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class USceneComponent;
class UNiagaraSystem;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class SLASHCPP_API AItem : public AActor
{
	GENERATED_BODY()
public:	
	AItem ();
	virtual void Tick (float DeltaTime) override;

	FVector RootComponentlocation;
	USceneComponent* rootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "sin Func Variables")
	bool CanIChangeFRotation = true;

	UPROPERTY(EditAnywhere, Category = "PickUp Properties|Effects")
	class UNiagaraComponent* ItemEffect;

private:
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly ,Category = "sin Func Variables", meta = (AllowPrivateAccess = "true"))
	float Runingtime;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category = "PickUp Properties|Effects")
	UNiagaraSystem* PickUpEffect;
	UPROPERTY(EditAnywhere, Category = "PickUp Properties|Effects")
	USoundBase* PickUpSound;

protected:
	
	virtual void BeginPlay() override;	

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY (EditAnywhere ,BlueprintReadWrite ,Category = "sin Func Variables")
	float Amplitude = 0.25f;
	UPROPERTY (EditAnywhere ,BlueprintReadWrite ,Category = "sin Func Variables")
	float TimeConstant = 5.f;
	UFUNCTION(BlueprintPure)
	virtual float TransformSin ();
	UFUNCTION (BlueprintPure)
	float TransformCos ();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float TotalRotation;

	template<typename T>
	T Avg (T Frist ,T Second);

	UFUNCTION()
	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	EItemState ItemState = EItemState::EIS_Hovering;


	virtual void SpawnSystem();
	virtual void SpawnPickUpSound();

};

template<typename T>
inline T AItem::Avg (T Frist ,T Second)
{
	return T (Frist + Second) / 2;
}