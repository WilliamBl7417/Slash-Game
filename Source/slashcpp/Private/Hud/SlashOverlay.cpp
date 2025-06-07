// Fill out your copyright notice in the Description page of Project Settings.


#include "Hud/SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashOverlay::SetHealthBarPercent(float percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(percent);
	}
}

void USlashOverlay::SetStaminaPercent(float percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(percent);
	}
}

void USlashOverlay::SetGold(int32 gold)
{
	if (GoldTExt)
	{
		const FString string = FString::Printf(TEXT("%d"), gold);
		const FText Text = FText::FromString(string);

		GoldTExt->SetText(Text);
	}
}

void USlashOverlay::SetSoul(int32 souls)
{
	if (SoulTExt)
	{
		const FString string = FString::Printf(TEXT("%d"), souls);
		const FText Text = FText::FromString(string);

		SoulTExt->SetText(Text);
	}
}