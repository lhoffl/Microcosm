#include "HealthbarWidget.h"

#include "UValueGauge.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "Components/SizeBox.h"

void UHealthbarWidget::UpdateHealthbarWidth() const
{
	const float MaxTotalHealth = OwningASC->GetNumericAttribute(UMAttributeSet::GetMaxTotalHealthAttribute());
	const float MaxHealth = OwningASC->GetNumericAttribute(UMAttributeSet::GetMaxHealthAttribute());
	const float MaxArmor = OwningASC->GetNumericAttribute(UMAttributeSet::GetMaxArmorAttribute());
	const float MaxOverhealth = OwningASC->GetNumericAttribute(UMAttributeSet::GetOverhealthAttribute());

	const float PercentageMaxHealth = MaxHealth / MaxTotalHealth;
	const float PercentageMaxArmor = MaxArmor / MaxTotalHealth;
	const float PercentageMaxOverhealth = MaxOverhealth / MaxTotalHealth;

	HealthBox->SetWidthOverride(HealthbarDefaultWidth * PercentageMaxHealth);
	ArmorBox->SetWidthOverride(HealthbarDefaultWidth * PercentageMaxArmor);
	OverhealthBox->SetWidthOverride(HealthbarDefaultWidth * PercentageMaxOverhealth);
}

void UHealthbarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(bConfigured) return;
	
	HealthBox->bOverride_WidthOverride = true;
	HealthBox->SetWidthOverride(HealthbarDefaultWidth);

	ArmorBox->bOverride_WidthOverride = true;
	ArmorBox->SetWidthOverride(HealthbarDefaultWidth);

	ShieldsBox->bOverride_WidthOverride = true;
	ShieldsBox->SetWidthOverride(HealthbarDefaultWidth);

	OverhealthBox->bOverride_WidthOverride = true;
	OverhealthBox->SetWidthOverride(HealthbarDefaultWidth);
}

void UHealthbarWidget::ConfigureWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	bConfigured = true;
	OwningASC = ASC;
	if(OwningASC)
	{
		HealthBar->SetAndBindGameplayAttribute(OwningASC, UMAttributeSet::GetHealthAttribute(), UMAttributeSet::GetMaxHealthAttribute());
		ArmorBar->SetAndBindGameplayAttribute(OwningASC, UMAttributeSet::GetArmorAttribute(), UMAttributeSet::GetMaxArmorAttribute());
		OverhealthBar->SetAndBindGameplayAttribute(OwningASC, UMAttributeSet::GetOverhealthAttribute(), UMAttributeSet::GetOverhealthAttribute());
		
		OwningASC->GetGameplayAttributeValueChangeDelegate(UMAttributeSet::GetMaxTotalHealthAttribute()).AddUObject(this, &UHealthbarWidget::MaxTotalHealthChanged);

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
