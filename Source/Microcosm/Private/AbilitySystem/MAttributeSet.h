// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "MAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	
	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	APawn* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	
	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	APawn* TargetCharacter = nullptr;
};

UCLASS()
class MICROCOSM_API UMAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UMAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	// VITAL
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes")
	FGameplayAttributeData Health;
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Attributes")
	FGameplayAttributeData Armor;
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxArmor, Category = "Attributes")
	FGameplayAttributeData MaxArmor;
	UFUNCTION()
	void OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxArmor);

	/*
	 *	Unlike health and armor, overhealth is temporary.
	 *	So we don't need to set a max; when it's gone it's gone for good.
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Overhealth, Category = "Attributes")
	FGameplayAttributeData Overhealth;
	UFUNCTION()
	void OnRep_Overhealth(const FGameplayAttributeData& OldOverhealth) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Overhealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalHealth, Category = "Attributes")
	FGameplayAttributeData TotalHealth;
	UFUNCTION()
	void OnRep_TotalHealth(const FGameplayAttributeData& OldTotalHealth) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, TotalHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxTotalHealth, Category = "Attributes")
	FGameplayAttributeData MaxTotalHealth;
	UFUNCTION()
	void OnRep_MaxTotalHealth(const FGameplayAttributeData& OldMaxTotalHealth) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxTotalHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Attributes")
	FGameplayAttributeData Stamina;
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LifestealPercentage, Category = "Attributes")
	FGameplayAttributeData LifestealPercentage;
	UFUNCTION()
	void OnRep_LifestealPercentage(const FGameplayAttributeData& OldLifestealPercentage) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, LifestealPercentage);
	
	// Primary

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen, Category = "Attributes")
	FGameplayAttributeData HealthRegen;
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaRegen, Category = "Attributes")
	FGameplayAttributeData StaminaRegen;
	UFUNCTION()
	void OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, StaminaRegen);	

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category = "Attributes")
	FGameplayAttributeData MoveSpeed;
	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MoveSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeedMultiplier, Category = "Attributes")
	FGameplayAttributeData MoveSpeedMultiplier;
	UFUNCTION()
	void OnRep_MoveSpeedMultiplier(const FGameplayAttributeData& OldMoveSpeedMultiplier) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MoveSpeedMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Attributes")
	FGameplayAttributeData AttackSpeed;
	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ReloadSpeed, Category = "Attributes")
	FGameplayAttributeData ReloadSpeed;
	UFUNCTION()
	void OnRep_ReloadSpeed(const FGameplayAttributeData& OldReloadSpeed) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, ReloadSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CooldownMultiplier, Category = "Attributes")
	FGameplayAttributeData CooldownMultiplier;
	UFUNCTION()
	void OnRep_CooldownMultiplier(const FGameplayAttributeData& OldCooldownMultiplier) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, CooldownMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IncomingHealingMultiplier, Category = "Attributes")
	FGameplayAttributeData IncomingHealingMultiplier;
	UFUNCTION()
	void OnRep_IncomingHealingMultiplier(const FGameplayAttributeData& OldIncomingHealingMultiplier) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, IncomingHealingMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OutgoingHealingMultiplier, Category = "Attributes")
	FGameplayAttributeData OutgoingHealingMultiplier;
	UFUNCTION()
	void OnRep_OutgoingHealingMultiplier(const FGameplayAttributeData& OldOutgoingHealingMultiplier) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, OutgoingHealingMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IncomingDamageMultiplier, Category = "Attributes")
	FGameplayAttributeData IncomingDamageMultiplier;
	UFUNCTION()
	void OnRep_IncomingDamageMultiplier(const FGameplayAttributeData& OldIncomingDamageMultiplier) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, IncomingDamageMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OutgoingDamageMultiplier, Category = "Attributes")
	FGameplayAttributeData OutgoingDamageMultiplier;
	UFUNCTION()
	void OnRep_OutgoingDamageMultiplier(const FGameplayAttributeData& OldOutgoingDamageMultiplier) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, OutgoingDamageMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IncomingCriticalDamageMultiplier, Category = "Attributes")
	FGameplayAttributeData IncomingCriticalDamageMultiplier;
	UFUNCTION()
	void OnRep_IncomingCriticalDamageMultiplier(const FGameplayAttributeData& OldIncomingCriticalDamageMultiplier) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, IncomingCriticalDamageMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OutgoingCriticalDamageMultiplier, Category = "Attributes")
	FGameplayAttributeData OutgoingCriticalDamageMultiplier;
	UFUNCTION()
	void OnRep_OutgoingCriticalDamageMultiplier(const FGameplayAttributeData& OldOutgoingCriticalDamageMultiplier) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, OutgoingCriticalDamageMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IncomingKnockbackMultiplier, Category = "Attributes")
	FGameplayAttributeData IncomingKnockbackMultiplier;
	UFUNCTION()
	void OnRep_IncomingKnockbackMultiplier(const FGameplayAttributeData& OldIncomingKnockbackMultiplier) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, IncomingKnockbackMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OutgoingKnockbackMultiplier, Category = "Attributes")
	FGameplayAttributeData OutgoingKnockbackMultiplier;
	UFUNCTION()
	void OnRep_OutgoingKnockbackMultiplier(const FGameplayAttributeData& OldOutgoingKnockbackMultiplier) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, OutgoingKnockbackMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ability1Charges, Category = "Attributes")
	FGameplayAttributeData Ability1Charges;
	UFUNCTION()
	void OnRep_Ability1Charges(const FGameplayAttributeData& OldAbility1Charges) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Ability1Charges);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ability2Charges, Category = "Attributes")
	FGameplayAttributeData Ability2Charges;
	UFUNCTION()
	void OnRep_Ability2Charges(const FGameplayAttributeData& OldAbility2Charges) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Ability2Charges);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ability3Charges, Category = "Attributes")
	FGameplayAttributeData Ability3Charges;
	UFUNCTION()
	void OnRep_Ability3Charges(const FGameplayAttributeData& OldAbility3Charges) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Ability3Charges);

	// Meta

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingHealing;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, IncomingHealing);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingSelfHealing;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, IncomingSelfHealing);

	// Ammo

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PrimaryAmmo, Category = "Ammo")
	FGameplayAttributeData PrimaryAmmo;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, PrimaryAmmo);
	UFUNCTION()
	void OnRep_PrimaryAmmo(const FGameplayAttributeData& OldPrimaryAmmo) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxPrimaryAmmo, Category = "Ammo")
	FGameplayAttributeData MaxPrimaryAmmo;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxPrimaryAmmo);
	UFUNCTION()
	void OnRep_MaxPrimaryAmmo(const FGameplayAttributeData& OldMaxPrimaryAmmo) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SecondaryAmmo, Category = "Ammo")
	FGameplayAttributeData SecondaryAmmo;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, SecondaryAmmo);
	UFUNCTION()
	void OnRep_SecondaryAmmo(const FGameplayAttributeData& OldSecondaryAmmo) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxSecondaryAmmo, Category = "Ammo")
	FGameplayAttributeData MaxSecondaryAmmo;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxSecondaryAmmo);
	UFUNCTION()
	void OnRep_MaxSecondaryAmmo(const FGameplayAttributeData& OldMaxSecondaryAmmo) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Charge, Category = "Attributes")
	FGameplayAttributeData Charge;
	UFUNCTION()
	void OnRep_Charge(const FGameplayAttributeData& OldCharge) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, Charge);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxCharge, Category = "Attributes")
	FGameplayAttributeData MaxCharge;
	UFUNCTION()
	void OnRep_MaxCharge(const FGameplayAttributeData& OldCharge) const;
	ATTRIBUTE_ACCESSORS(UMAttributeSet, MaxCharge);

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void UpdateFullHealthTag(const UAbilitySystemComponent* ASC) const;
	void CalculateAndSetTotalHealth(const UAbilitySystemComponent* ASC);
};

