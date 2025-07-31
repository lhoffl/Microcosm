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
	virtual void Die();
	virtual void InitAbilityActorInfo();
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	virtual void HandleDeath();
	virtual void SetupHitboxes();
	void AddCharacterAbilities() const;
	virtual void PossessedBy(AController* NewController) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void BindCallbacksToDependencies();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHandleDeath();

	FCollisionQueryParams GetIgnoreCharacterParams() const;

	bool bIsDead;

	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Ability and Attribute Defaults|Class")
	ECharacterClass CharacterClass;	

	//virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "UI|Overhead")
	UWidgetComponent* OverheadWidget;

	bool IsLocallyControlledByPlayer() const;
	
	void ConfigureOverheadStatusWidget();
	virtual void OnFullHealthTagChanged(FGameplayTag GameplayTag, signed int NewCount) const;

	virtual void ServerSideInit();
	virtual void ClientSideInit();

	UPROPERTY(EditDefaultsOnly, Category = "UI|Overhead")
	float OverheadStatVisibilityTimerPeriod = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Overhead")
	float OverheadStatVisibilityMaxRangeSquared = 10000000.f;
	
	FTimerHandle OverheadStatVisibilityTimerHandle;
	FTimerHandle FullHealthOverheadDelayTimerHandle;
	FDelegateHandle FullHealthTagChangedDelegate;

	void UpdateOverheadStatVisibility() const;

	virtual void Destroyed() override;
	
	UPROPERTY()
	UOverheadStatusGauge* OverheadStatusGauge;

	void ScaleOverheadStatusGauge() const;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Overhead")
	float MinOverheadScale = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Overhead")
	float MaxOverheadScale = 2000.f;

	UPROPERTY()
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	void SetAIPerceptionStimuliSourceEnabled(bool bIsEnabled) const;

	bool bShowName;
	bool bAlwaysShowHealth = true;
	bool bCallbacksBound;

	UFUNCTION()
	virtual void OnRep_TeamID();

	UPROPERTY(ReplicatedUsing = OnRep_TeamID)
	FGenericTeamId TeamID;
};
