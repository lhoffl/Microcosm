// copyright lhoffl


#include "MGameplayTags.h"

#include "GameplayTagsManager.h"
FMGameplayTags FMGameplayTags::GameplayTags;

void FMGameplayTags::InitializeNativeGameplayTags()
{
	// Vital
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"));
	GameplayTags.Attributes_Vital_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.MaxHealth"));

	// Primary
	GameplayTags.Attributes_Primary_HealthRegen = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.HealthRegen"));
	GameplayTags.Attributes_Primary_MoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MoveSpeed"));
	GameplayTags.Attributes_Primary_MaxMoveSpeed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MaxMoveSpeed"));
	GameplayTags.Attributes_Primary_MoveSpeedMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MoveSpeedMultiplier"));
	
	GameplayTags.Attributes_Primary_IncomingHealingMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.IncomingHealingMultiplier"));
	GameplayTags.Attributes_Primary_OutgoingHealingMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.OutgoingHealingMultiplier"));
	GameplayTags.Attributes_Primary_IncomingDamageMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.IncomingDamageMultiplier"));
	GameplayTags.Attributes_Primary_OutgoingDamageMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.OutgoingDamageMultiplier"));
	GameplayTags.Attributes_Primary_IncomingKnockbackMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.IncomingKnockbackMultiplier"));
	GameplayTags.Attributes_Primary_OutgoingKnockbackMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.OutgoingKnockbackMultiplier"));

	// Input	
	GameplayTags.InputTag_PrimaryFire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.PrimaryFire"));
	GameplayTags.InputTag_SecondaryFire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.SecondaryFire"));
	GameplayTags.InputTag_Ability1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Ability1"));
	
	// Effects
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"));
	GameplayTags.Effects_Knockback = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.Knockback"));
	GameplayTags.Effects_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.Stun"));

	GameplayTags.Status_Invulnerable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Status.Invulnerable"));

	GameplayTags.State_Movement_Fireball = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Movement.Fireball"));
	GameplayTags.State_Movement_InAir = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Movement.InAir"));
	GameplayTags.State_Movement_Disable_MovementInputs = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Movement.Disable.MovementInputs"));
	GameplayTags.State_Movement_Grounded = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Movement.Grounded"));
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
	GameplayTags.Abilities_ReloadActive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.ReloadActive"));
	GameplayTags.Abilities_ActiveCard_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.ActiveCard.Active"));
	GameplayTags.Abilities_ActiveCard_Blocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.ActiveCard.Blocked"));
	
	// Passive
	GameplayTags.Abilities_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive"));
	GameplayTags.Abilities_Passive_SelfHealing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.SelfHealing"));
	GameplayTags.Abilities_Passive_SelfHealing_Delay = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.SelfHealing.Delay"));
	GameplayTags.Abilities_Passive_SelfHealing_Amount = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.SelfHealing.Amount"));
	GameplayTags.Abilities_Passive_SelfHealing_Rate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.SelfHealing.Rate"));
	
	// Socket / Montage
	GameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Weapon"));

	/* 
	*	Cues
	*/

	// Common
	GameplayTags.Cue_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.HitReact"));
	GameplayTags.Cue_Hit_Generic = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Hit.Generic"));

	GameplayTags.Cue_State_Movement_Fireball_Started = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Movement.Fireball.Started"));
	GameplayTags.Cue_State_Movement_Fireball_Loop = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Movement.Fireball.Loop"));
	GameplayTags.Cue_State_Movement_Fireball_End = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Movement.Fireball.End"));
}