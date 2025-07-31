// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UMAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag&);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDealtDamageWithAbility, const FGameplayTagContainer&, Tags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDealtHealingWithAbility, const FGameplayTagContainer&, Tags);
DECLARE_MULTICAST_DELEGATE_OneParam(FDealtDamageWithAbilityNative, const FGameplayTagContainer& Tags);
DECLARE_MULTICAST_DELEGATE_OneParam(FDealtHealingWithAbilityNative, const FGameplayTagContainer& Tags);

UCLASS()
class UMAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	
	UPROPERTY(BlueprintAssignable, Category = "Microcosm|Events")
	FDealtDamageWithAbility DealtDamageWithAbility;

	UPROPERTY(BlueprintAssignable, Category = "Microcosm|Events")
	FDealtHealingWithAbility DealtHealingWithAbility;

	FDealtDamageWithAbilityNative DealtDamageWithAbilityNative;
	FDealtHealingWithAbilityNative DealtHealingWithAbilityNative;	

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& DefaultAbilities);
	FGameplayAbilitySpecHandle AddAbility(const TSubclassOf<UGameplayAbility>& InAbility, const int32 InLevel = 1, const bool bRemoveOnEnd = false);
	bool bStartupAbilitiesGiven;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetCooldownTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	
	UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters) const;

	UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void AddGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters) const;

	UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters) const;
	
protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) const;

	virtual void OnRep_ActivateAbilities() override;	
};
