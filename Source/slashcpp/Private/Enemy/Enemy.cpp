#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "slashcpp/Public/AttributeComponent/AttributeComponent.h"
#include "Hud/HealthBarComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Items/Sword.h"
#include "Items/Soul.h"

AEnemy::AEnemy()
{
 	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar Component"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
	GetCharacterMovement()->MaxWalkSpeed = 125.f;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensing Component"));
	PawnSensing->SightRadius = 45.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	EnemyState = EEnemyState::EAS_Patrolling;
}
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isDead()) return;
	if (EnemyState > EEnemyState::EAS_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

		// Ignorar daño si proviene de otro enemigo
	if (DamageCauser && DamageCauser->ActorHasTag("Enemy"))
	{
		return 0.f;
	}

	if (bIsDead) return 0.f;

	// Solo establecer como objetivo si es el jugador
	APawn* InstigatorPawn = EventInstigator ? EventInstigator->GetPawn() : nullptr;
	if (InstigatorPawn && InstigatorPawn->ActorHasTag("EngageableTarget"))
	{
		CombatTarget = InstigatorPawn;
	}
	HandleDamage(DamageAmount);
	ChaseTarget();

	if (IsInsideAttackCombatRange())
	{
		EnemyState = EEnemyState::EAS_Attacking;
	}
	else if (IsOutsideAttackRange())
	{
		ChaseTarget();
	}
	return DamageAmount;
}
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!isDead())showHealthBar();
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	stopAttackMontage();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Green, FString(TEXT("hitted")));
	}
}
bool AEnemy::IsAlive() const
{
	return EnemyState != EEnemyState::EAS_Dead;
}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	//Tags.Add(FName("Enemy"));
	Tags.Add(FName("Enemy"));
	if (PawnSensing)PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeem);
	InitializeEnemy();
}
void AEnemy::Die()
{
	Super::Die();
	bIsDead = true;
	EnemyState = EEnemyState::EAS_Dead;
	hideHealthBar();
	ClearAttackTimer();
	DisableCapsule();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &AEnemy::DestroyActor, 5.f, false);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnSoul();
}
void AEnemy::SpawnSoul()
{
	UWorld* world = GetWorld();
	if (world && SoulClass)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);

		ASoul* Spawnedsoul = world->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (Spawnedsoul && Attribute)
		{
			Spawnedsoul->SetSouls(Attribute->GetSouls());
			Spawnedsoul->SetOwner(this);
		}
	}
}
void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	PlayAttackmontage();
	EnemyState = EEnemyState::EAS_Engaged;
}
bool AEnemy::canAttack()
{
	bool bCanAttack = 
		IsInsideAttackCombatRange() && 
		!IsAttacking() && 
		!IsEngaged() &&
		!isDead();
	return bCanAttack;
}
void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EAS_NoState;
	CheckCombatTarget();
}
void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attribute && HealthBarWidget) {
		HealthBarWidget->SetHealthPercent(Attribute->GetHealthPercent());
		// Verificar salud directamente (no porcentaje)
		if (Attribute->GetHealth() <= 0.f && !bIsDead) 
		{
			Die();
		}
	}
}
void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());
	hideHealthBar();
	MoveToTarget(PatroTarget);
	SpawnDefaultWeapon();
}
void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatroTarget, PatrolRadius))
	{
		int32 TimeRandom = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		PatroTarget = ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, TimeRandom);
	}
	switch (EnemyState)
	{
	case EEnemyState::EAS_Patrolling:
		GetCharacterMovement()->MaxWalkSpeed = 150.f;
		break;
	case EEnemyState::EAS_Chasing:
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		break;
	default:
		break;
	}
}
void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRange())
	{
		ClearAttackTimer();
		loseInterest();
		if (!IsEngaged()) startPatrolling();
	}
	else if (IsOutsideAttackRange() && !IsChasing())
	{
		ClearAttackTimer();
		if(!IsEngaged()) ChaseTarget();
	}
	else if (canAttack())
	{
		StartAttackTimer();
	}
}
void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatroTarget);
}
void AEnemy::hideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}
void AEnemy::showHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}
void AEnemy::loseInterest()
{
	CombatTarget = nullptr;
	hideHealthBar();
}
void AEnemy::startPatrolling()
{
	EnemyState = EEnemyState::EAS_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatroTarget);
}
void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EAS_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
	showHealthBar();
}
bool AEnemy::IsOutsideCombatRange()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}
bool AEnemy::IsOutsideAttackRange()
{
	return !InTargetRange(CombatTarget, AttackingRadius);
}
bool AEnemy::IsInsideAttackCombatRange()
{
	return InTargetRange(CombatTarget, AttackingRadius);
}
bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EAS_Chasing;
}
bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EAS_Attacking;
}
bool AEnemy::isDead()
{
	return EnemyState == EEnemyState::EAS_Dead;
}
bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EAS_Engaged;
}
void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}
void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EAS_Attacking;
	const float AttackTime = FMath::RandRange(attackMinTime, attackMaxTime);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}
void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}
bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (!Target) return false;

	const double SquaredDistance = (Target->GetActorLocation() - GetActorLocation()).SquaredLength();
	return SquaredDistance <= (Radius * Radius);
}
void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptancegRadius);
	EnemyController->MoveTo(MoveRequest);
}
AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (auto Target : PatrolTargets)
	{
		if (Target != PatroTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 numPatrolTarget = ValidTargets.Num();
	if (numPatrolTarget > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, numPatrolTarget - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}
void AEnemy::SpawnDefaultWeapon()
{
	UWorld* world = GetWorld();
	if (world && WeaponClass)
	{
		ASword* DefaultWeapon = world->SpawnActor<ASword>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
		EquippedWeapon->CanIChangeFRotation = false;
	}
}
void AEnemy::DestroyActor()
{
	// Desactivar colisiones y física antes de destruir
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	Destroy();
}
void AEnemy::PawnSeem(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EAS_Dead &&
		EnemyState != EEnemyState::EAS_Chasing &&
		EnemyState < EEnemyState::EAS_Attacking &&
		SeenPawn->ActorHasTag(FName("EngageableTarger"));
	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}