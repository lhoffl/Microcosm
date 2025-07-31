#pragma once
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "AttributeTextWidget.generated.h"

UCLASS()
class UAttributeTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	void SetAndBindGameplayAttribute(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute);
	void SetValue(float NewValue) const;
	void ValueChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	
private:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	class UTextBlock* ValueText;

	float CachedValue;
};