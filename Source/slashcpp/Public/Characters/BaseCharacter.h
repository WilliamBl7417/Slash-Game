// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"

class ASword;
class UAttributeComponent;
class UAnimMontage;


UCLASS()
class SLASHCPP_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Constructor
	ABaseCharacter();

	// AActor Overrides
	virtual void Tick(float DeltaTime) override;

	// IHitInterface Override
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// Public Functions
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	// AActor Overrides
	virtual void BeginPlay() override;

	// Combat & Health Functions
	virtual bool IsAlive() const; // Declared twice in original header, removed duplicate.
	virtual void HandleDamage(float DamageAmount);
	virtual void Die();
	void StartRagDoll();
	void DisableCapsule();
	void DirectionalHitReaction(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);

	// Attack & Dodge Functions
	virtual bool canAttack();
	virtual void Attack();
	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	// Animation & Montage Functions
	void PlayHitReactMontage(const FName& SectionName);
	void playMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	virtual int32 PlayAttackmontage();
	virtual int32 PlayDeathmontage();
	virtual void PlayDogeMontage();
	void stopAttackMontage();

	// Warp Target Functions
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	// Properties
	UPROPERTY(EditDefaultsOnly, Category = "Character Properties|Hit Reaction Montages")
	UAnimMontage* HitReacMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Character Properties|Attack Montages")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Character Properties|Attack Montages")
	TArray<FName> AttackMontageSections;
	UPROPERTY(EditDefaultsOnly, Category = "Character Properties|Death Montages")
	TArray<FName> DeathMontageSections;
	UPROPERTY(EditDefaultsOnly, Category = "Character Properties|Death Montages")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Character Properties|Dodge Montage")
	UAnimMontage* DodgeMontage;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(VisibleAnywhere, Category = "Character Properties|Weapon")
	ASword* EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attribute;

	UPROPERTY(BlueprintReadOnly)
	AActor* CombatTarget;
	UPROPERTY(EditAnywhere)
	double WarpTargetDistance = 75.f;

private:
	// Private Properties
	UPROPERTY(EditAnywhere, Category = "Character Properties|Combact Effects")
	UParticleSystem* HitParticles;
	UPROPERTY(EditAnywhere, Category = "Character Properties|Combact Effects")
	USoundBase* HitSound;
};