#pragma once
#include "AttributeSet.h"
#include "AbilitySystem/MAttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "UValueGauge.generated.h"

UCLASS()
class UValueGauge : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	void SetAndBindGameplayAttribute(class UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute, const FGameplayAttribute& MaxAttribute);
	void SetValue(float NewValue, float NewMaxValue) const;
	
private:
	UPROPERTY(EditAnywhere, Category = "Visuals")
	FLinearColor FillColor;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	class UProgressBar* ProgressBar;

	void ValueChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void MaxValueChanged(const FOnAttributeChangeData& OnAttributeChangeData);

	float CachedValue;
	float CachedMaxValue;
};
