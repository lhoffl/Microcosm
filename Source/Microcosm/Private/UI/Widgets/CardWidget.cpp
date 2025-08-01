// copyright lhoffl


#include "UI/Widgets/CardWidget.h"

void UCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DefaultSizeBoxHeight = SizeBox->HeightOverride;
	DefaultSizeBoxWidth = SizeBox->WidthOverride;
}

void UCardWidget::SetActiveCard(const bool bIsActive)
{
	if(bIsActive)
	{
		SizeBox->HeightOverride = DefaultSizeBoxHeight * ActiveSizeBoxHeightOverridePercentage;
		SizeBox->WidthOverride = DefaultSizeBoxWidth * ActiveSizeBoxWidthOverridePercentage;

		Image->Brush.TintColor = ActiveCardColor;
	}
	else
	{
		SizeBox->HeightOverride = DefaultSizeBoxHeight;
		SizeBox->WidthOverride = DefaultSizeBoxWidth;
		Image->Brush.TintColor = UnactiveCardColor;
	}
}

void UCardWidget::SetColor(FLinearColor Color)
{
	Image->SetColorAndOpacity(Color);
}
