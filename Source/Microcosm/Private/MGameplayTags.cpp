// copyright lhoffl


#include "MGameplayTags.h"

#include "GameplayTagsManager.h"
FMGameplayTags FMGameplayTags::GameplayTags;

void FMGameplayTags::InitializeNativeGameplayTags()
{
	// Vital
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"));
	GameplayTags.Attributes_Vital_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.MaxHealth"));
	GameplayTags.Attributes_Vital_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Armor"));
	GameplayTags.Attributes_Vital_MaxArmor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.MaxArmor"));
	GameplayTags.Attributes_Vital_Overhealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Overhealth"));
	GameplayTags.Attributes_Vital_MaxOverhealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.MaxOverhealth"));
	GameplayTags.Attributes_Vital_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Stamina"));
	GameplayTags.Attributes_Vital_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.MaxStamina"));

	// Primary
	GameplayTags.Attributes_Primary_HealthRegen = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.HealthRegen"));
	GameplayTags.Attributes_Primary_StaminaRegen = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.StaminaRegen"));
	GameplayTags.Attributes_Primary_MoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MoveSpeed"));
	GameplayTags.Attributes_Primary_MaxMoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MaxMoveSpeed"));
	GameplayTags.Attributes_Primary_MoveSpeedMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MoveSpeedMultiplier"));
	GameplayTags.Attributes_Primary_AttackSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.AttackSpeed"));
	GameplayTags.Attributes_Primary_ReloadSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.ReloadSpeed"));
	GameplayTags.Attributes_Primary_CooldownMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.CooldownMultiplier"));
	
	GameplayTags.Attributes_Primary_IncomingHealingMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.IncomingHealingMultiplier"));
	GameplayTags.Attributes_Primary_OutgoingHealingMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.OutgoingHealingMultiplier"));
	GameplayTags.Attributes_Primary_IncomingDamageMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.IncomingDamageMultiplier"));
	GameplayTags.Attributes_Primary_OutgoingDamageMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.OutgoingDamageMultiplier"));
	GameplayTags.Attributes_Primary_IncomingCriticalDamageMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.IncomingCriticalDamageMultiplier"));
	GameplayTags.Attributes_Primary_OutgoingCriticalDamageMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.OutgoingCriticalDamageMultiplier"));
	GameplayTags.Attributes_Primary_IncomingKnockbackMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.IncomingKnockbackMultiplier"));
	GameplayTags.Attributes_Primary_OutgoingKnockbackMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.OutgoingKnockbackMultiplier"));

	GameplayTags.Abilities_RefundCooldownOnElimination = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.RefundCooldownOnElimination"));

	// Input	
	GameplayTags.InputTag_PrimaryFire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.PrimaryFire"));
	GameplayTags.InputTag_SecondaryFire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.SecondaryFire"));
	GameplayTags.InputTag_Ability1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Ability1"));
	GameplayTags.InputTag_Ability2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Ability2"));
	GameplayTags.InputTag_Ultimate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Ultimate"));
	GameplayTags.InputTag_Reload = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Reload"));
	GameplayTags.InputTag_Interact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Interact"));
	GameplayTags.InputTag_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Melee"));
	GameplayTags.InputTag_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Jump"));
	GameplayTags.InputTag_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Crouch"));
	
	// Effects
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"));
	GameplayTags.Effects_Knockback = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.Knockback"));
	GameplayTags.Effects_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.Stun"));

	GameplayTags.Status_Invulnerable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Status.Invulnerable"));
	GameplayTags.Status_Unhealable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Status.Unhealable"));
	GameplayTags.Status_Aim_Invalid = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Status.Aim.Invalid"));

	GameplayTags.State_Movement_InAir = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Movement.InAir"));
	GameplayTags.State_Movement_Grounded = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Movement.Grounded"));
	GameplayTags.State_Movement_Crouched = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Movement.Crouched"));
	GameplayTags.State_Movement_Sliding = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Movement.Sliding"));
	GameplayTags.State_FullHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.FullHealth"));
	GameplayTags.State_Dying = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Dying"));
	GameplayTags.State_Dead = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Dead"));

	// Damage
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"));
	GameplayTags.RecentlyTookDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("RecentlyTookDamage"));

	// Healing
	GameplayTags.Healing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Healing"));

	// Abilities
	GameplayTags.Cooldown_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.None"));
	GameplayTags.Abilities_BlockMyOtherAbilitiesFromCasting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.BlockMyOtherAbilitiesFromCasting"));

	GameplayTags.Abilities_Reload = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Reload"));
	GameplayTags.Abilities_ReloadActive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.ReloadActive"));
	GameplayTags.Abilities_BlockReload = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.BlockReload"));
	GameplayTags.Abilities_CancelReload = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.CancelReload"));
	
	GameplayTags.Abilities_MaxPrimaryAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.MaxPrimaryAmmo"));
	GameplayTags.Abilities_PrimaryAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.PrimaryAmmo"));
	GameplayTags.Abilities_SecondaryAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.SecondaryAmmo"));
	GameplayTags.Abilities_MaxSecondaryAmmo = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.MaxSecondaryAmmo"));

	GameplayTags.Weapon_PrimaryAmmo_Cost = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.PrimaryAmmo.Cost"));
	GameplayTags.Weapon_SecondaryAmmo_Cost = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.SecondaryAmmo.Cost"));
	GameplayTags.Weapon_PrimaryAmmo_FireDelay = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.PrimaryAmmo.FireDelay"));
	GameplayTags.Weapon_SecondaryAmmo_FireDelay = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.SecondaryAmmo.FireDelay"));
	GameplayTags.Weapon_PrimaryAmmo_MoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.PrimaryAmmo.MoveSpeed"));
	GameplayTags.Weapon_SecondaryAmmo_MoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.SecondaryAmmo.MoveSpeed"));
	GameplayTags.Weapon_Reload_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.Reload.Duration"));
	GameplayTags.Weapon_Reload = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.Reload"));
	GameplayTags.Weapon_PrimaryAmmo_Firing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.PrimaryAmmo.Firing"));
	GameplayTags.Weapon_SecondaryAmmo_Firing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.SecondaryAmmo.Firing"));

	GameplayTags.Abilities_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.1"));
	GameplayTags.Abilities_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.2"));
	GameplayTags.Abilities_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.3"));
	GameplayTags.Abilities_1_Charges = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.1.Charges"));
	GameplayTags.Abilities_2_Charges = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.2.Charges"));
	GameplayTags.Abilities_3_Charges = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.3.Charges"));
	
	GameplayTags.Attributes_Charge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Charge"));
	GameplayTags.Attributes_MaxCharge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.MaxCharge"));

	GameplayTags.Abilities_Movement_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Movement.Jump"));
	GameplayTags.Abilities_Movement_Jump_Double = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Movement.Jump.Double"));
	GameplayTags.Abilities_Movement_Jump_Wall = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Movement.Jump.Wall"));
	GameplayTags.Abilities_Movement_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Movement.Crouch"));
	GameplayTags.Abilities_Movement_Crouch_Slide = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Movement.Crouch.Slide"));
	
	// Passive
	GameplayTags.Abilities_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive"));
	GameplayTags.Abilities_CanLifesteal = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.CanLifesteal"));
	GameplayTags.Abilities_Passive_SelfHealing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.SelfHealing"));
	GameplayTags.Abilities_Passive_SelfHealing_Delay = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.SelfHealing.Delay"));
	GameplayTags.Abilities_Passive_SelfHealing_Amount = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.SelfHealing.Amount"));
	GameplayTags.Abilities_Passive_SelfHealing_Rate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.SelfHealing.Rate"));
	GameplayTags.Abilities_Passive_Lifesteal = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Lifesteal"));
	GameplayTags.Abilities_Passive_Lifesteal_Amount = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Lifesteal.Amount"));
	
	// Socket / Montage
	GameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Weapon"));
	GameplayTags.Montage_Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.RightHand"));
	GameplayTags.Montage_Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.LeftHand"));

	/* 
	*	Cues
	*/

	// Common
	GameplayTags.Cue_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.HitReact"));
	GameplayTags.Cue_Hit_Generic = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Hit.Generic"));
	
}