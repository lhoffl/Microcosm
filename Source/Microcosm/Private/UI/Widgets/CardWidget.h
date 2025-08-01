// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "CardWidget.generated.h"

UCLASS()
class UCardWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	USizeBox* SizeBox;

	UPROPERTY(meta=(BindWidget))
	UImage* Image;

	UPROPERTY(EditDefaultsOnly)
	float ActiveSizeBoxHeightOverridePercentage = 1.50;

	UPROPERTY(EditDefaultsOnly)
	float ActiveSizeBoxWidthOverridePercentage = 1.25;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor UnactiveCardColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor ActiveCardColor;
	
	float DefaultSizeBoxHeight;
	float DefaultSizeBoxWidth;

	UFUNCTION(BlueprintCallable)
	void SetActiveCard(bool bIsActive);

	UFUNCTION(BlueprintCallable)
	void SetColor(FLinearColor Color);
};
