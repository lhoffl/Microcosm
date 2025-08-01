#pragma once
#include "AbilitySystem/MAttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "HealthbarWidget.generated.h"

class UAbilitySystemComponent;

UCLASS()
class UHealthbarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateHealthbarWidth() const;
	virtual void NativeConstruct() override;
	void ConfigureWithAbilitySystem(UAbilitySystemComponent* ASC);
	void SetIsEnemyHealthbar(const FLinearColor Color) const;
	
private:
	UPROPERTY(meta=(BindWidget))
	class UValueGauge* HealthBar;

	UPROPERTY(meta=(BindWidget))
	class USizeBox* HealthBox;

	UPROPERTY()
	UAbilitySystemComponent* OwningASC;

	UPROPERTY(EditDefaultsOnly)
	float HealthbarDefaultWidth = 200.f;

	void MaxTotalHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;
	
	/*
	 *	Delay the configuration of the widget one frame to ensure server host gets valid values.
	 *	Not required for clients or dedicated server.
	 */
	void EnsureValidHealth() const;

	bool bConfigured = false;
};

