// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/SlashHUD.h"
#include "Hud/SlashOverlay.h"

void ASlashHUD::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && SlasOverlayClass)
		{
			SlashOverlay = CreateWidget<USlashOverlay>(Controller, SlasOverlayClass);
			SlashOverlay->AddToViewport();
		}
	}
}
