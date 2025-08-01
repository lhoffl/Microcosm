#include "HealthbarWidget.h"

#include "UValueGauge.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "Components/SizeBox.h"

void UHealthbarWidget::UpdateHealthbarWidth() const
{
	HealthBox->SetWidthOverride(HealthbarDefaultWidth);
}

void UHealthbarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(bConfigured) return;
	
	HealthBox->bOverride_WidthOverride = true;
	HealthBox->SetWidthOverride(HealthbarDefaultWidth);
}

void UHealthbarWidget::ConfigureWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	bConfigured = true;
	OwningASC = ASC;
	if(OwningASC)
	{
		HealthBar->SetAndBindGameplayAttribute(OwningASC, UMAttributeSet::GetHealthAttribute(), UMAttributeSet::GetMaxHealthAttribute());
		
		OwningASC->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UHealthbarWidget::MaxTotalHealthChanged);

		UpdateHealthbarWidth();
		EnsureValidHealth();
	}
	else
	{
		UMAbilitySystemLibrary::OnScreenDebugMessage("No ASC Healthbar!");
	}
}

void UHealthbarWidget::MaxTotalHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	UpdateHealthbarWidth();
}

void UHealthbarWidget::EnsureValidHealth() const
{
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		UpdateHealthbarWidth();
	});
}

void UHealthbarWidget::SetIsEnemyHealthbar(const FLinearColor Color) const
{
	HealthBar->SetColorAndOpacity(Color);
}
