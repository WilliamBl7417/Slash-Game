// Fill out your copyright notice in the Description page of Project Settings.
#include "Item.h"
//#include "slashcpp/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "Interfaces/PickUpInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItem::AItem (): Runingtime(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent> (TEXT ("ItemMeshComponent"));
	ItemMesh->SetUsingAbsoluteRotation(false);
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//RootComponent = ItemMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Item Effect"));
	ItemEffect->SetupAttachment(GetRootComponent());

}

void AItem::BeginPlay ()
{
	Super::BeginPlay ();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::onSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::onSphereEndOverlap);
}

float AItem::TransformSin ()
{
	return Amplitude * FMath::Sin (Runingtime * TimeConstant);
}

float AItem::TransformCos()
{
	return Amplitude * FMath::Cos (Runingtime * TimeConstant);
}

void AItem::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString otheractorvar = OtherActor->GetName();

	IPickUpInterface* PickUpInterace = Cast<IPickUpInterface>(OtherActor);

	if (PickUpInterace)
	{
		PickUpInterace->SetOverlapingItem(this);
	}
}

void AItem::onSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString otheractorvar = OtherActor->GetName();
	IPickUpInterface* PickUpInterace = Cast<IPickUpInterface>(OtherActor);

	if (PickUpInterace)
	{
		PickUpInterace->SetOverlapingItem(this);
	}
}

void AItem::SpawnSystem()
{
	if (PickUpEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickUpEffect,
			GetActorLocation()
		);
	}
}

void AItem::SpawnPickUpSound()
{
	if (PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			PickUpSound,
			GetActorLocation()
		);
	}
}



void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//RootComponentlocation = GetRootComponent()->GetComponentLocation();
	//Draw_Sphere_SingleFrame(RootComponentlocation);

	if (CanIChangeFRotation)
	{
		Runingtime += DeltaTime;

		float RotationSpeed = 0.0f;
		// Calcula la rotación a agregar en este fotograma
		float RotationThisFrame = RotationSpeed * DeltaTime;
		TotalRotation += RotationThisFrame;

		// Aplica la rotación al objeto
		FRotator NewRotation = FRotator(TotalRotation, 0.f, 0.f);
		SetActorRotation(NewRotation);

		if (ItemState == EItemState::EIS_Hovering)
		{
			AddActorLocalOffset(FVector(0.f, 0.f, TransformSin()));
		}
	}
}


