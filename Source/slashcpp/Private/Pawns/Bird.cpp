// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Engine.h"
// Sets default values
ABird::ABird()
{
 PrimaryActorTick.bCanEverTick = true;

 Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
 Capsule->SetCapsuleHalfHeight(20.f);
 Capsule->SetCapsuleRadius(20.f);
 RootComponent = Capsule;

 SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
 SkeletalMesh->SetupAttachment(Capsule);

 SprintArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
 SprintArm->SetupAttachment(GetRootComponent());
 SprintArm->TargetArmLength = 300.f;

 CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
 CameraComponent->SetupAttachment(SprintArm);

 AutoPossessPlayer = EAutoReceiveInput::Player0;//setea que el jugador posea este obvjeto al iniciar

}

void ABird::BeginPlay()
{
	Super::BeginPlay();
	
}

//function for the movement of the paw
void ABird::MoveForward(float Value)
{

	//print the value off the input
/*	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Green, FString::Printf(TEXT("Valor: %f"), Value));
	}
*/

	//add movement to the pawn
	if ((Controller != nullptr) && (Value != 0.f))//ask if the controller exist, and also if the value is not 0
	{
		FVector Forward = GetActorForwardVector();//save forwar vector in a variable
		AddMovementInput(Forward, Value);//call the function to add movement and enter this parametters
	}
}

void ABird::Turn(float Value)
{
	AddControllerYawInput(Value);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Green, FString::Printf(TEXT("Valor: %f"), Value));
	}
}

void ABird::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//bindea el imput del jugador, a este pawn y llama la funcion 

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABird::LookUp);

}


