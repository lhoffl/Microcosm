// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/MAbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "UI/Widgets/OverheadStatusGauge.h"
#include "MCharacter.generated.h"

class UMAbilitySystemComponent;

UCLASS()
class AMCharacter : public APawn, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMCharacter();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	virtual UMAbilitySystemComponent* GetMAbilitySystemComponent() const { return  Cast<UMAbilitySystemComponent>(AbilitySystemComponent); }
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void Tick(float DeltaTime) override;
	void EliminationSecured();
	virtual void InitAbilityActorInfo();
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities() const;
	virtual void PossessedBy(AController* NewController) override;

	void BindCallbacksToDependencies();
	
	FCollisionQueryParams GetIgnoreCharacterParams() const;

	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Ability and Attribute Defaults|Class")
	ECharacterClass CharacterClass;	

	UPROPERTY(VisibleDefaultsOnly, Category = "UI|Overhead")
	UWidgetComponent* OverheadWidget;

	bool IsLocallyControlledByPlayer() const;
	
	virtual void ServerSideInit();

	UPROPERTY(EditDefaultsOnly, Category = "UI|Overhead")
	float OverheadStatVisibilityTimerPeriod = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Overhead")
	float OverheadStatVisibilityMaxRangeSquared = 10000000.f;
	
	FTimerHandle OverheadStatVisibilityTimerHandle;
	FTimerHandle FullHealthOverheadDelayTimerHandle;
	FDelegateHandle FullHealthTagChangedDelegate;

	virtual void Destroyed() override;
	
	UPROPERTY()
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	void SetAIPerceptionStimuliSourceEnabled(bool bIsEnabled) const;

	bool bCallbacksBound;

	UPROPERTY()
	FGenericTeamId TeamID;
};
