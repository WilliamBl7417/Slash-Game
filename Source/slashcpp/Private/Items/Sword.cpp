																																																														// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Sword.h"
#include "Characters/SlashCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

ASword::ASword()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(ItemMesh);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBox->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponBox->SetGenerateOverlapEvents(true);
	BoxTraceStar = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStar->SetupAttachment(ItemMesh);
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(ItemMesh);
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnBoxOverlap);
}

void ASword::Equip(USceneComponent* InParent, FName InSocketName, AActor* newOwner, APawn* newInstigato)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(newOwner);
	SetInstigator(newInstigato);
	AttachMesToSocket(InParent, InSocketName);
}

void ASword::AttachMesToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransforRules(EAttachmentRule::SnapToTarget, true);
	GetRootComponent()->AttachToComponent(InParent, TransforRules, InSocketName);
}

void ASword::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Verificación básica de seguridad
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;

	// Obtener el dueño del arma
	AActor* WeaponOwner = GetOwner();

	if (!WeaponOwner) return;

	// Si el dueño es un enemigo y el otro actor es el jugador, o viceversa
	bool bOwnerIsEnemy = WeaponOwner->ActorHasTag("Enemy");
	bool bOtherIsPlayer = OtherActor->ActorHasTag("EngageableTarger");

	// Solo procesar si es un enemigo golpeando al jugador o jugador golpeando enemigo
	if ((bOwnerIsEnemy && bOtherIsPlayer) || (!bOwnerIsEnemy && OtherActor->ActorHasTag("Enemy")))
	{
		FHitResult BoxHit;
		BoxTrace(BoxHit);

		if (BoxHit.GetActor() && BoxHit.GetActor() != this && BoxHit.GetActor() != GetOwner())
		{
			UGameplayStatics::ApplyDamage(
				BoxHit.GetActor(),
				Damage,
				GetInstigator()->GetController(),
				this,
				UDamageType::StaticClass()
			);
			ExecuteGetHit(BoxHit);
			CreateFields(BoxHit.ImpactPoint);
		}
	}
}

void ASword::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		//HitInterface->GetHit(BoxHit.ImpactPoint);
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint,GetOwner());
	}
}
void ASword::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStar->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorToIgnore;

	ActorToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorToIgnore.AddUnique(Actor);
	}
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtend,
		BoxTraceStar->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}


