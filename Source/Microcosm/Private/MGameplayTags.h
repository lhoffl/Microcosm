// copyright lhoffl

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FMGameplayTags
{
public:
	static const FMGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	//Vital
	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_MaxHealth;
	FGameplayTag Attributes_Vital_Armor;
	FGameplayTag Attributes_Vital_MaxArmor;
	FGameplayTag Attributes_Vital_Overhealth;
	FGameplayTag Attributes_Vital_MaxOverhealth;
	FGameplayTag Attributes_Vital_Stamina;
	FGameplayTag Attributes_Vital_MaxStamina;
	
	//Primary 
	FGameplayTag Attributes_Primary_HealthRegen;
	FGameplayTag Attributes_Primary_StaminaRegen;
	FGameplayTag Attributes_Primary_MoveSpeed;
	FGameplayTag Attributes_Primary_MaxMoveSpeed;
	FGameplayTag Attributes_Primary_MoveSpeedMultiplier;
	FGameplayTag Attributes_Primary_AttackSpeed;
	FGameplayTag Attributes_Primary_ReloadSpeed;
	FGameplayTag Attributes_Primary_CooldownMultiplier;
	
	FGameplayTag Attributes_Primary_IncomingHealingMultiplier;
	FGameplayTag Attributes_Primary_OutgoingHealingMultiplier;
	FGameplayTag Attributes_Primary_IncomingDamageMultiplier;
	FGameplayTag Attributes_Primary_OutgoingDamageMultiplier;
	FGameplayTag Attributes_Primary_IncomingCriticalDamageMultiplier;
	FGameplayTag Attributes_Primary_OutgoingCriticalDamageMultiplier;
	FGameplayTag Attributes_Primary_IncomingKnockbackMultiplier;
	FGameplayTag Attributes_Primary_OutgoingKnockbackMultiplier;

	// Input
	FGameplayTag InputTag_PrimaryFire;	
	FGameplayTag InputTag_SecondaryFire;	
	FGameplayTag InputTag_Reload;	
	FGameplayTag InputTag_Melee;	
	FGameplayTag InputTag_Interact;	
	FGameplayTag InputTag_Ability1;	
	FGameplayTag InputTag_Ability2;	
	FGameplayTag InputTag_Ultimate;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Crouch;

	// Effects
	FGameplayTag Effects_HitReact;
	FGameplayTag Effects_Stun;
	FGameplayTag Effects_Knockback;
	FGameplayTag Status_Invulnerable;
	FGameplayTag Status_Unhealable;
	FGameplayTag Status_Aim_Invalid;
	
	FGameplayTag State_Movement_InAir;
	FGameplayTag State_Movement_Grounded;
	FGameplayTag State_Movement_Crouched;
	FGameplayTag State_Movement_Sliding;
	FGameplayTag State_Dying;
	FGameplayTag State_Dead;
	FGameplayTag State_FullHealth;

	// Damage
	FGameplayTag Damage;
	FGameplayTag RecentlyTookDamage;


	// Healing
	FGameplayTag Healing;

	// Abilities Common
	FGameplayTag Cooldown_None;
	FGameplayTag Abilities_BlockMyOtherAbilitiesFromCasting;
	FGameplayTag Abilities_Reload;
	FGameplayTag Abilities_ReloadActive;
	FGameplayTag Abilities_BlockReload;
	FGameplayTag Abilities_CancelReload;
	FGameplayTag Abilities_PrimaryAmmo;
	FGameplayTag Abilities_MaxPrimaryAmmo;
	FGameplayTag Abilities_SecondaryAmmo;
	FGameplayTag Abilities_MaxSecondaryAmmo;
	FGameplayTag Abilities_1;
	FGameplayTag Abilities_2;
	FGameplayTag Abilities_3;
	FGameplayTag Abilities_1_Charges;
	FGameplayTag Abilities_2_Charges;
	FGameplayTag Abilities_3_Charges;
	FGameplayTag Abilities_RefundCooldownOnElimination;
	FGameplayTag Abilities_Movement_Jump;
	FGameplayTag Abilities_Movement_Jump_Double;
	FGameplayTag Abilities_Movement_Jump_Wall;
	FGameplayTag Abilities_Movement_Crouch;
	FGameplayTag Abilities_Movement_Crouch_Slide;
	
	// Weapon
	FGameplayTag Weapon_PrimaryAmmo_Cost;
	FGameplayTag Weapon_SecondaryAmmo_Cost;
	FGameplayTag Weapon_PrimaryAmmo_FireDelay;
	FGameplayTag Weapon_SecondaryAmmo_FireDelay;
	FGameplayTag Weapon_Reload_Duration;
	FGameplayTag Weapon_Reload;
	FGameplayTag Weapon_PrimaryAmmo_MoveSpeed;
	FGameplayTag Weapon_SecondaryAmmo_MoveSpeed;
	FGameplayTag Weapon_PrimaryAmmo_Firing;
	FGameplayTag Weapon_SecondaryAmmo_Firing;
	
	FGameplayTag Attributes_Charge;
	FGameplayTag Attributes_MaxCharge;

	// Passives

	FGameplayTag Abilities_Passive;
	FGameplayTag Abilities_Passive_SelfHealing;
	FGameplayTag Abilities_Passive_SelfHealing_Delay;
	FGameplayTag Abilities_Passive_SelfHealing_Amount;
	FGameplayTag Abilities_Passive_SelfHealing_Rate;
	FGameplayTag Abilities_Passive_Lifesteal;
	FGameplayTag Abilities_CanLifesteal;
	FGameplayTag Abilities_Passive_Lifesteal_Amount;
	
	// Montage
	FGameplayTag Montage_Attack_Weapon;
	FGameplayTag Montage_Attack_RightHand;
	FGameplayTag Montage_Attack_LeftHand;

	/*
	 *	Cues
	 */

	// Common
	FGameplayTag Cue_HitReact;
	FGameplayTag Cue_Hit_Generic;
	
private:
	static FMGameplayTags GameplayTags;
};
