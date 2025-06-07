#include "Hud/HealthBarComponent.h"
#include "Components/ProgressBar.h"
#include "Hud/HealthBar.h"

void UHealthBarComponent::SetHealthPercent(float percent)
{
	//way to avoid casting all the time
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	if (HealthBarWidget && HealthBarWidget->Health_Bar)
	{
		HealthBarWidget->Health_Bar->SetPercent(percent);	
	}
}
