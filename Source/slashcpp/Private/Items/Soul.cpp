// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interfaces/PickUpInterface.h"
#include "Kismet/KismetSystemLibrary.h"


ASoul::ASoul()
{

}
void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const double LocationZ = GetActorLocation().Z;
	if (LocationZ > DesiredZ)
	{
		const FVector DeltaLocation = FVector(0.f, 0.f, -DriftSpeed * DeltaTime);

		AddActorWorldOffset(DeltaLocation);
	}
}
void ASoul::BeginPlay()
{
	Super::BeginPlay();
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	DesiredZ = HitResult.ImpactPoint.Z;
}
void ASoul::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	const FString otheractorvar = OtherActor->GetName();
	IPickUpInterface* PickUpInterace = Cast<IPickUpInterface>(OtherActor);

	if (PickUpInterace)
	{
		PickUpInterace->AddSouls(this);
		SpawnSystem();
		SpawnPickUpSound();
		Destroy();
	}
}


