// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickUpInterface.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class ASoul;
class ATreasure;
class UAnimMontage;
class ASword;
class UCombatComponent; // This class is declared but not used in the provided .cpp. Consider removing if not needed.
class USlashOverlay;

UCLASS()
class SLASHCPP_API ASlashCharacter : public ABaseCharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:
	// Constructor
	ASlashCharacter();

	// AActor Overrides
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ACharacter Overrides
	virtual void Jump() override;

	// ABaseCharacter Overrides
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual bool canAttack() override;
	virtual void Die() override;
	virtual void DodgeEnd() override;
	virtual bool IsAlive() const override;

	// IPickUpInterface Overrides
	virtual void SetOverlapingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* soul) override;
	virtual void AddGold(ATreasure* Treasure) override;

	// Public Functions
	UFUNCTION(BlueprintCallable)
	void ChangeState(EActionState NewState);

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }


protected:
	// AActor Overrides
	virtual void BeginPlay() override;

	// Input Callbacks
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EKeyPressed();
	void Dodge();

	// Combat & Equipment Functions
	void EquipWeapon(ASword* Weapon);
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();
	void PlayEquipMontage(FName SectionName);
	bool HaveWeapon();
	bool canDisarm();
	bool canArm();
	void disarm();
	void arm();

	// State and Condition Checks
	bool HasEnoughStamina();
	bool IsUnoccupied();


private:
	// Character Components
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere)
	UGroomComponent* HairMesh;
	UPROPERTY(VisibleAnywhere)
	UGroomComponent* EyeBrowsMesh;

	// Character Related Variables
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlapingItem;
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	USlashOverlay* SlashOverlay;

	// Private Helper Functions
	void InitializeSlashOverlay();
	void SetHUDHealth();
};