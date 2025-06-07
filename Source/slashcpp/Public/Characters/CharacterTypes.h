#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_Death UMETA(DisplayName = "Dead"),
	ECS_EquippedOnHandedWeapon UMETA(DisplayName = "Equipped one Handed weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped two handed weapon"),
	ECS_EquippedOnBack UMETA(DisplayName = "Equipped on back")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "EquipingWeapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Death UMETA(DisplayName = "Death")
};
UENUM(BlueprintType)
enum class EEnemyState : uint8
{	
	EAS_NoState        UMETA(DisplayName = "NoState"),
	EAS_Dead           UMETA(DisplayName = "Dead"),
	EAS_Patrolling     UMETA(DisplayName = "Patrolling"),
	EAS_Chasing        UMETA(DisplayName = "Chasing"),
	EAS_Attacking      UMETA(DisplayName = "Attacking"),
	EAS_Engaged        UMETA(DisplayName = "Engaged")
	
};

UENUM(BlueprintType)
enum EDeathPose
{
	EDP_DeadFront   UMETA(DisplayName = "DeadFront"),
	EDP_DeadBack    UMETA(DisplayName = "DeadBack"),
	EDP_MAX         UMETA(DisplayName = "DefaultMAX")
};
