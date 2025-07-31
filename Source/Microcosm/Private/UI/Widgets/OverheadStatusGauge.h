#pragma once
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "OverheadStatusGauge.generated.h"

UCLASS()
class UOverheadStatusGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	void ConfigureWithAbilitySystem(UAbilitySystemComponent* ASC);
	void UpdateName(const FString& PlayerName) const;
	void SetHealthbarVisibility(bool InVisibility) const;
	void SetNameTextVisibility(bool InVisibility) const;
	void SetIsEnemyHealthbar(const bool bIsEnemy) const;
	
private:
	UPROPERTY(meta=(BindWidget))
	class UHealthbarWidget* HealthBar;	

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NameText;

	UPROPERTY()
	UAbilitySystemComponent* OwningASC;

	UPROPERTY(EditDefaultsOnly, Category = "Healthbar|Enemy")
	FLinearColor EnemyColor;

	UPROPERTY(EditDefaultsOnly, Category = "Healthbar|Ally")
	FLinearColor AllyColor;
};
