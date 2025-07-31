#include "UValueGauge.h"

#include "Components/ProgressBar.h"

void UValueGauge::NativePreConstruct()
{
	Super::NativePreConstruct();
	ProgressBar->SetFillColorAndOpacity(FillColor);
}

void UValueGauge::SetAndBindGameplayAttribute(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute, const FGameplayAttribute& MaxAttribute)
{
	if(!ASC) return;
	bool bFound;
	float Value = ASC->GetGameplayAttributeValue(Attribute, bFound);
	float MaxValue = ASC->GetGameplayAttributeValue(MaxAttribute, bFound);

	if(bFound)
	{
		CachedValue = Value;
		CachedMaxValue = MaxValue;
		SetValue(Value, MaxValue);
	}

	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UValueGauge::ValueChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &UValueGauge::MaxValueChanged);
}

void UValueGauge::SetValue(const float NewValue, const float NewMaxValue) const
{
	if (NewMaxValue <= 0) return;

	const float NewPercent = NewValue / NewMaxValue;
	ProgressBar->SetPercent(NewPercent);
}

void UValueGauge::ValueChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	CachedValue = OnAttributeChangeData.NewValue;
	SetValue(CachedValue, CachedMaxValue);	
}

void UValueGauge::MaxValueChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	CachedMaxValue = OnAttributeChangeData.NewValue;
	SetValue(CachedValue, CachedMaxValue);
}