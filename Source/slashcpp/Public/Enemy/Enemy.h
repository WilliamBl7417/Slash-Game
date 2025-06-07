// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class SLASHCPP_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/** <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, 
		struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator,
		AActor* DamageCauser) override;
	/** <AActor> */

	/**<IHitInterface>*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/**<IHitInterface>*/

	UPROPERTY(BlueprintReadWrite)
	EEnemyState EnemyState = EEnemyState::EAS_Patrolling;

protected:
	virtual bool IsAlive() const override;

	/**<ABaseCharacter>*/
	virtual void BeginPlay() override;
	virtual void Die() override;
	void SpawnSoul();
	virtual void Attack() override;
	virtual bool canAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	/**<ABaseCharacter>*/

	/**<AI Behaivior variables>*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties|Combat")
	double CombatRadius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties|Combat")
	double AttackingRadius = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties|Combat")
	double AcceptancegRadius = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties|Combat")
	float attackMinTime = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties|Combat")
	float attackMaxTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties|Navigations")
	float PatrollingSpeed = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties|Navigations")
	float ChasingSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Properties|Navigations")
	double PatrolRadius = 200.f;
	UPROPERTY(EditInstanceOnly, Category = "Character Properties|Navigations")
	AActor* PatroTarget;
	UPROPERTY(EditInstanceOnly, Category = "Character Properties|Navigations")
	TArray<AActor*> PatrolTargets;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)//Component
	UPawnSensingComponent* PawnSensing;
	UPROPERTY()
	class AAIController* EnemyController;

	
	FTimerHandle DestroyTimer;
	bool bIsDead = false;
	/**<AI Behaivior variables>*/

private:

	/**<AI Behaivior functions>*/
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void hideHealthBar();
	void showHealthBar();
	void loseInterest();
	void startPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRange();
	bool IsOutsideAttackRange();
	bool IsInsideAttackCombatRange();
	bool IsChasing();
	bool IsAttacking();
	bool isDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();
	void DestroyActor();
	UFUNCTION()
	void PawnSeem(APawn* SeenPawn);
	/**<AI Behaivior functions>*/


	/**<AI variables>*/
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;
	/**<AI variables>*/


	/**<Weapon variables>*/
	UPROPERTY(EditAnywhere, Category = "Character Properties|Combat")
	TSubclassOf<class ASword> WeaponClass;
	/**<Weapon variables>*/


	/**<AI Behaivior variables>*/

	FTimerHandle AttackTimer;
	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere, Category = "Character Properties|Navigations|WaitingTimes")
	float PatrolWaitMin = 2;
	UPROPERTY(EditAnywhere, Category = "Character Properties|Navigations|WaitingTimes")
	float PatrolWaitMax = 7;
	/**<AI Behaivior variables>*/

	UPROPERTY(EditAnywhere, Category = "Character Properties|Combat")
	TSubclassOf<class ASoul> SoulClass;
};
