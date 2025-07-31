// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "AbilitySystem/MAbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Input/MInputConfig.h"
#include "MPlayerController.generated.h"

UCLASS()
class AMPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	//void SpawnGameplayWidget();
	virtual void AcknowledgePossession(APawn* InPawn) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> MContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> PlayActiveCardAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UMInputConfig> InputConfig;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void PlayActiveCard(const FInputActionValue& InputActionValue);
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	FHitResult CursorHit;
	bool bTargeting;

	void CursorTrace();

	UPROPERTY()
	TObjectPtr<UMAbilitySystemComponent> MAbilitySystemComponent;

	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<class UGameplayWidget> GameplayWidgetClass;

	//UPROPERTY()
	//UGameplayWidget* GameplayWidget;
	
	UMAbilitySystemComponent* GetASC();	
};
