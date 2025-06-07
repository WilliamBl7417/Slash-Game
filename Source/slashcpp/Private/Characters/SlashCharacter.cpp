#include "Characters/SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h" // This header is included but StaticMeshComponent is not directly used for this character.
#include "GroomComponent.h"
#include "AttributeComponent/AttributeComponent.h"
#include "Item.h"
#include "Items/Sword.h"
#include "Animation/AnimMontage.h"
#include "Hud/SlashHUD.h"
#include "Hud/SlashOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

// Constructor
ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Sping Arm Component"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	HairMesh = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair mesh"));
	HairMesh->SetupAttachment(GetMesh());
	HairMesh->AttachmentName = FString("Head");

	EyeBrowsMesh = CreateDefaultSubobject<UGroomComponent>(TEXT("Eye Brows"));
	EyeBrowsMesh->SetupAttachment(GetMesh());
	EyeBrowsMesh->AttachmentName = FString("Head");
}

// AActor Overrides
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("EngageableTarger"));
	Tags.Add(FName("Player"));

	InitializeSlashOverlay();
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Attribute && SlashOverlay)
	{
		Attribute->RegenStamina(DeltaTime);
		SlashOverlay->SetStaminaPercent(Attribute->GetStaminaPercent());
	}
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ASlashCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Atttack"), IE_Pressed, this, &ASlashCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &ASlashCharacter::Dodge);
}

// ACharacter Overrides
void ASlashCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

// ABaseCharacter Overrides
bool ASlashCharacter::IsAlive() const
{
	return Attribute && !Attribute->IsDead() &&
		ActionState != EActionState::EAS_Death &&
		CharacterState != ECharacterState::ECS_Death;
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	ActionState = EActionState::EAS_HitReaction;
}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ASlashCharacter::Attack()
{
	Super::Attack();
	if (canAttack())
	{
		PlayAttackmontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::canAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void ASlashCharacter::Die()
{
	Super::Die();

	ActionState = EActionState::EAS_Death;
	CharacterState = ECharacterState::ECS_Death;

	DisableCapsule();
	DisableInput(GetWorld()->GetFirstPlayerController());

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Green, FString(TEXT("muerto")));
	}
}

void ASlashCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

// IPickUpInterface Overrides
void ASlashCharacter::SetOverlapingItem(AItem* Item)
{
	OverlapingItem = Item;
}

void ASlashCharacter::AddSouls(ASoul* soul)
{
	if (Attribute && SlashOverlay)
	{
		Attribute->AddSouls(soul->GetSouls());
		SlashOverlay->SetSoul(Attribute->GetSouls());
	}
}

void ASlashCharacter::AddGold(ATreasure* Treasure)
{
	if (Attribute && SlashOverlay)
	{
		Attribute->AddGold(Treasure->GetGold());
		SlashOverlay->SetGold(Attribute->GetGold());
	}
}

// Public Functions
void ASlashCharacter::ChangeState(EActionState NewState)
{
	ActionState = NewState;
}

// Input Callbacks
void ASlashCharacter::MoveForward(float Value)
{
	//si el estado es atacar no se puede mover
	if (ActionState == EActionState::EAS_Attacking) return;//devuelve return earlier

	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void ASlashCharacter::MoveRight(float Value)
{
	if (Controller && (Value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void ASlashCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ASlashCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ASlashCharacter::EKeyPressed()
{
	ASword* OverlapingWeapong = Cast<ASword>(OverlapingItem);

	if (OverlapingWeapong)
	{
		EquipWeapon(OverlapingWeapong);
	}
	else if (HaveWeapon())
	{
		if (canArm())
		{
			//esto fuere refactorizado en  el video 8.finishing refactoritation
			arm();
		}
		else if (canDisarm())
		{
			//esto fuere refactorizado en  el video 8.finishing refactoritation
			disarm();
		}
	}
}

void ASlashCharacter::Dodge()
{
	if (IsUnoccupied() && !HasEnoughStamina()) return;

	PlayDogeMontage();

	ActionState = EActionState::EAS_Dodge;

	if (Attribute && SlashOverlay)
	{
		Attribute->UsedStamina(Attribute->GetDodgeCost());
		SlashOverlay->SetStaminaPercent(Attribute->GetStaminaPercent());
	}
}

// Combat & Equipment Functions
void ASlashCharacter::EquipWeapon(ASword* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);

	CharacterState = ECharacterState::ECS_EquippedOnHandedWeapon;
	OverlapingItem = nullptr;
	EquippedWeapon = Weapon;
	Weapon->CanIChangeFRotation = false;
}

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMesToSocket(GetMesh(), FName("spine_Socket"));
	}
}

void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMesToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

bool ASlashCharacter::HaveWeapon()
{
	return CharacterState == ECharacterState::ECS_EquippedOnHandedWeapon || CharacterState == ECharacterState::ECS_EquippedOnBack;
}

bool ASlashCharacter::canDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::canArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void ASlashCharacter::disarm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOnHandedWeapon;
	//ActionState = EActionState::EAS_EquippingWeapon; // This line is commented out in original, keeping it commented.
}

void ASlashCharacter::arm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	//ActionState = EActionState::EAS_EquippingWeapon; // This line is commented out in original, keeping it commented.
}

// State and Condition Checks
bool ASlashCharacter::HasEnoughStamina()
{
	return Attribute && Attribute->GetStamina() > Attribute->GetDodgeCost();
}

bool ASlashCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

// Private Helper Functions
void ASlashCharacter::InitializeSlashOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		ASlashHUD* SlashHud = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHud)
		{
			SlashOverlay = SlashHud->GetSlashOverlay();
			if (SlashOverlay && Attribute)
			{
				SlashOverlay->SetHealthBarPercent(Attribute->GetHealthPercent());
				SlashOverlay->SetStaminaPercent(1.f);
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSoul(0);
			}
		}
	}
}

void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attribute)
	{
		SlashOverlay->SetHealthBarPercent(Attribute->GetHealthPercent());
	}
}