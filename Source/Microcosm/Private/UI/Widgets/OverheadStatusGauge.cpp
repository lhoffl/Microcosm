#include "OverheadStatusGauge.h"

#include "HealthbarWidget.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/TextBlock.h"

void UOverheadStatusGauge::ConfigureWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	OwningASC = ASC;
	if(OwningASC) HealthBar->ConfigureWithAbilitySystem(OwningASC);
	else UMAbilitySystemLibrary::OnScreenDebugMessage("No ASC set in Overhead Status Widget");
}

void UOverheadStatusGauge::UpdateName(const FString& PlayerName) const
{
	NameText->SetText(FText::FromString(PlayerName));
}

void UOverheadStatusGauge::SetHealthbarVisibility(const bool InVisibility) const
{
	const ESlateVisibility SlateVisibility = InVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	HealthBar->SetVisibility(SlateVisibility);
}

void UOverheadStatusGauge::SetNameTextVisibility(const bool InVisibility) const
{
	const ESlateVisibility SlateVisibility = InVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	NameText->SetVisibility(SlateVisibility);
}

void UOverheadStatusGauge::SetIsEnemyHealthbar(const bool bIsEnemy) const
{
	if(bIsEnemy)
	{
		HealthBar->SetIsEnemyHealthbar(EnemyColor);
		NameText->SetColorAndOpacity(EnemyColor);
	}
	else
	{
		HealthBar->SetIsEnemyHealthbar(AllyColor);
		NameText->SetColorAndOpacity(AllyColor);
	}
}