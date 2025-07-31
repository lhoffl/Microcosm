#include "AttributeTextWidget.h"

#include "Components/TextBlock.h"

void UAttributeTextWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UAttributeTextWidget::SetAndBindGameplayAttribute(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute)
{
	if(!ASC) return;
	bool bFound;

	const float Value = ASC->GetGameplayAttributeValue(Attribute, bFound);
	if(bFound)
	{
		CachedValue = Value;
		SetValue(Value);
	}

	if(Attribute.IsValid()) ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UAttributeTextWidget::ValueChanged);
}

void UAttributeTextWidget::SetValue(const float NewValue) const
{
	const FNumberFormattingOptions FormattingOptions = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	ValueText->SetText(FText::Format(FTextFormat::FromString("{0}"), FText::AsNumber(NewValue, &FormattingOptions)));
}

void UAttributeTextWidget::ValueChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	CachedValue = OnAttributeChangeData.NewValue;
	SetValue(CachedValue);	
}