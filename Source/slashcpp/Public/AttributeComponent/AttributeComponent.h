// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASHCPP_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);

protected:

	virtual void BeginPlay() override;
private:
	//Current Health of the actor
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health = 100;
	
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 100;

	//Current Stamina of the actor
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 14.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 8.f;

public:

	void ReceiveDamage(float Damage);
	void UsedStamina(float StaminaCost);
	float GetHealthPercent();
	float GetStaminaPercent();


	float GetHealth() const { return Health; }
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 amoundOfGold);


	FORCEINLINE float GetHealthPercent() const { return Health / MaxHealth; }
	FORCEINLINE bool IsDead() const { return Health <= 0.f; }
	
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE int32 GetGold()const { return Gold; }
	FORCEINLINE int32 GetSouls()const { return Souls; }
};
