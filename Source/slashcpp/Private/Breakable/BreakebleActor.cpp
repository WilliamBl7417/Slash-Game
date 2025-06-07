// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakebleActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Characters/SlashCharacter.h"





ABreakebleActor::ABreakebleActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	CapsuleColition = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collition"));
	CapsuleColition->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	CapsuleColition->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	CapsuleColition->SetupAttachment(GetRootComponent());


}

void ABreakebleActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakebleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakebleActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	UWorld* world = GetWorld();
	
	if (world && ItemToSpawn.Num() > 0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;

		const int32 Selection = FMath::RandRange(0, ItemToSpawn.Num() - 1);
		GetWorld()->SpawnActor<AActor>(ItemToSpawn[Selection] , Location, GetActorRotation());
	

	}
}

