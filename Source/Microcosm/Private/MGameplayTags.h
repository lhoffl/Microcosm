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
	
	//Primary 
	FGameplayTag Attributes_Primary_HealthRegen;
	FGameplayTag Attributes_Primary_MoveSpeed;
	FGameplayTag Attributes_Primary_MaxMoveSpeed;
	FGameplayTag Attributes_Primary_MoveSpeedMultiplier;
	
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
	FGameplayTag InputTag_Ability1;	
	FGameplayTag InputTag_Jump;	

	// Effects
	FGameplayTag Effects_HitReact;
	FGameplayTag Effects_Stun;
	FGameplayTag Effects_Knockback;
	FGameplayTag Status_Invulnerable;
	
	FGameplayTag State_Movement_InAir;
	FGameplayTag State_Movement_Fireball;
	FGameplayTag State_Movement_Disable_MovementInputs;
	FGameplayTag State_Movement_Grounded;
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
	FGameplayTag Abilities_ReloadActive;
	FGameplayTag Abilities_ActiveCard_Active;
	FGameplayTag Abilities_ActiveCard_Blocked;
	
	// Passives

	FGameplayTag Abilities_Passive;
	FGameplayTag Abilities_Passive_SelfHealing;
	FGameplayTag Abilities_Passive_SelfHealing_Delay;
	FGameplayTag Abilities_Passive_SelfHealing_Amount;
	FGameplayTag Abilities_Passive_SelfHealing_Rate;
	
	// Montage
	FGameplayTag Montage_Attack_Weapon;

	/*
	 *	Cues
	 */

	// Common
	FGameplayTag Cue_HitReact;
	FGameplayTag Cue_Hit_Generic;
	
	FGameplayTag Cue_State_Movement_Fireball_Started;
	FGameplayTag Cue_State_Movement_Fireball_Loop;
	FGameplayTag Cue_State_Movement_Fireball_End;
	
private:
	static FMGameplayTags GameplayTags;
};
