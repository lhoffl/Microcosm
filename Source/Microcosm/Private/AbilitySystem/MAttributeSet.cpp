// copyright lhoffl


#include "AbilitySystem/MAttributeSet.h"

// copyright lhoffl

#include "AbilitySystem/MAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "MGameplayTags.h"
#include "AbilitySystem/MAbilitySystemLibrary.h"
#include "Character/MCharacter.h"
#include "Game/MGameMode.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Player/MPlayerController.h"

struct FMGameplayTags;

UMAttributeSet::UMAttributeSet()
{
	const FMGameplayTags& GameplayTags = FMGameplayTags::Get();

	// Vital
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_MaxHealth, GetMaxHealthAttribute);	
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_MaxArmor, GetMaxArmorAttribute);	
	
	// Primary
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_HealthRegen, GetHealthRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MoveSpeed, GetMoveSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MoveSpeedMultiplier, GetMoveSpeedMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_AttackSpeed, GetAttackSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_ReloadSpeed, GetReloadSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_CooldownMultiplier, GetCooldownMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_IncomingHealingMultiplier, GetIncomingHealingMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_OutgoingHealingMultiplier, GetOutgoingHealingMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_IncomingDamageMultiplier, GetIncomingDamageMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_OutgoingDamageMultiplier, GetOutgoingDamageMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_IncomingCriticalDamageMultiplier, GetIncomingCriticalDamageMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_OutgoingCriticalDamageMultiplier, GetOutgoingCriticalDamageMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_IncomingKnockbackMultiplier, GetIncomingKnockbackMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_OutgoingKnockbackMultiplier, GetOutgoingKnockbackMultiplierAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Charge, GetChargeAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_MaxCharge, GetMaxChargeAttribute);

	// Ammo
	TagsToAttributes.Add(GameplayTags.Abilities_PrimaryAmmo, GetPrimaryAmmoAttribute);
	TagsToAttributes.Add(GameplayTags.Abilities_MaxPrimaryAmmo, GetMaxPrimaryAmmoAttribute);
	TagsToAttributes.Add(GameplayTags.Abilities_SecondaryAmmo, GetSecondaryAmmoAttribute);
	TagsToAttributes.Add(GameplayTags.Abilities_MaxSecondaryAmmo, GetMaxSecondaryAmmoAttribute);
}

void UMAttributeSet::UpdateFullHealthTag(const UAbilitySystemComponent* ASC) const
{
	if (!ASC || !ASC->IsOwnerActorAuthoritative()) return;
	
	const FGameplayTag& FullHealthTag = FMGameplayTags::Get().State_FullHealth;
	if(GetTotalHealth() == GetMaxTotalHealth())
	{
		if(!ASC->HasMatchingGameplayTag(FullHealthTag))
		{
			UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(ASC->GetOwnerActor(), FullHealthTag.GetSingleTagContainer(), true);
		}
	}
	else
	{
		if(ASC->HasMatchingGameplayTag(FullHealthTag))
		{
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(ASC->GetOwnerActor(), FullHealthTag.GetSingleTagContainer(), true);
		}
	}
}

void UMAttributeSet::CalculateAndSetTotalHealth(const UAbilitySystemComponent* ASC)
{
	const float MaxTotal = GetMaxHealth() + GetMaxArmor() + GetOverhealth();
	const float Total = GetHealth() + GetArmor() + GetOverhealth();

	SetMaxTotalHealth(MaxTotal);
	SetTotalHealth(FMath::Clamp(Total, 0.f, MaxTotal));

	UpdateFullHealthTag(ASC);
}

void UMAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(GetHealth() <= 0.f) return;
	
	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	/*if(Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		SetMoveSpeed(FMath::Clamp(GetMoveSpeed() * GetMoveSpeedMultiplier(), 0.f, UMAbilitySystemLibrary::GetMaxMoveSpeedForCurrentGameMode(Props.TargetCharacter)));
	}

	if(Data.EvaluatedData.Attribute == GetMoveSpeedMultiplierAttribute())
	{
		const float BaseSpeed = Props.TargetASC->GetNumericAttributeBase(GetMoveSpeedAttribute());
		const float NewSpeed = FMath::Clamp(BaseSpeed * GetMoveSpeedMultiplier(), 0.f, UMAbilitySystemLibrary::GetMaxMoveSpeedForCurrentGameMode(Props.TargetCharacter));
		SetMoveSpeed(NewSpeed);
	}*/
	
	if(Data.EvaluatedData.Attribute == GetMaxTotalHealthAttribute())
	{
		SetTotalHealth(FMath::Clamp(GetTotalHealth(), 0.f, GetMaxTotalHealth()));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}

	if(Data.EvaluatedData.Attribute == GetTotalHealthAttribute())
	{
		SetTotalHealth(FMath::Clamp(GetTotalHealth(), 0.f, GetMaxTotalHealth()));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}
	
	if(Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}

	if(Data.EvaluatedData.Attribute == GetMaxArmorAttribute())
	{
		SetArmor(FMath::Clamp(GetArmor(), 0.f, GetMaxArmor()));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}
	
	if(Data.EvaluatedData.Attribute == GetPrimaryAmmoAttribute())
	{
		SetPrimaryAmmo(FMath::Clamp(GetPrimaryAmmo(), 0.f, GetMaxPrimaryAmmo()));
	}

	if(Data.EvaluatedData.Attribute == GetSecondaryAmmoAttribute())
	{
		SetSecondaryAmmo(FMath::Clamp(GetSecondaryAmmo(), 0.f, GetMaxSecondaryAmmo()));
	}
	
	if(Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Max(GetMaxHealth(), 0.f));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}

	if(Data.EvaluatedData.Attribute == GetArmorAttribute())
	{
		SetArmor(FMath::Clamp(GetArmor(), 0.f, GetMaxArmor()));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}

	if(Data.EvaluatedData.Attribute == GetOverhealthAttribute())
	{
		SetOverhealth(FMath::Max(GetOverhealth(), 0.f));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}

	if(Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	
	if(Data.EvaluatedData.Attribute == GetChargeAttribute())
	{
		SetCharge(FMath::Clamp(GetCharge(), 0.f, GetMaxCharge()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if(LocalIncomingDamage > 0.f)
		{
			float CurrentArmor = GetArmor();
			float CurrentEffectiveArmor = CurrentArmor * 2;
			float Remainder = LocalIncomingDamage;
			float DamageMultiplier = GetIncomingDamageMultiplier() * Props.SourceASC->GetNumericAttribute(GetOutgoingDamageMultiplierAttribute());

			Remainder *= DamageMultiplier;
			CurrentEffectiveArmor -= Remainder;
			CurrentEffectiveArmor /= 2;

			SetArmor(FMath::Clamp(CurrentEffectiveArmor, 0.f, GetMaxArmor()));
			CalculateAndSetTotalHealth(Props.TargetASC);
			Remainder -= CurrentArmor * 2;
			
			if(Remainder <= 0)
			{
				CalculateAndSetTotalHealth(Props.TargetASC);
				return;
			}
			
			const float NewHealth = GetHealth() - Remainder;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			CalculateAndSetTotalHealth(Props.TargetASC);

			const UMAbilitySystemComponent* MASC = Cast<UMAbilitySystemComponent>(Props.SourceASC);
			FGameplayTagContainer ActorTags;
			FGameplayTagContainer SpecTags;
			Props.EffectContextHandle.GetOwnedGameplayTags(ActorTags, SpecTags);
			MASC->DealtDamageWithAbility.Broadcast(ActorTags);
			MASC->DealtDamageWithAbilityNative.Broadcast(ActorTags);
			
			const float DamageModifier = GetIncomingDamageMultiplier() * Props.SourceASC->GetNumericAttribute(GetOutgoingDamageMultiplierAttribute());
			
			float RealDamageDealt = LocalIncomingDamage * DamageModifier;
			if(NewHealth <= 0)
			{
				RealDamageDealt = NewHealth + LocalIncomingDamage;
			}

			if(SpecTags.HasTagExact(FMGameplayTags::Get().Abilities_CanLifesteal))
			{
				const float LifestealAmount = RealDamageDealt * Props.SourceASC->GetNumericAttribute(GetLifestealPercentageAttribute());
				UMAbilitySystemLibrary::ApplyInstantDynamicEffect(Props.SourceASC, GetIncomingHealingAttribute(), LifestealAmount);
			}
			
			const bool bFatal = NewHealth <= 0.f;

			if(bFatal)
			{
				if(AMGameMode* MGameMode = GetWorld()->GetAuthGameMode<AMGameMode>())
				{
					AMPlayerController* SourcePlayerController = Cast<AMPlayerController>(Props.SourceController);
					AMPlayerController* TargetPlayerController = Cast<AMPlayerController>(Props.TargetController);
					AMCharacter* TargetCharacter = Cast<AMCharacter>(Props.TargetCharacter);

					MGameMode->PlayerEliminated(TargetCharacter, TargetPlayerController, SourcePlayerController);	
				}
			}
			else
			{
				Props.TargetASC->TryActivateAbilitiesByTag(FMGameplayTags::Get().Effects_HitReact.GetSingleTagContainer());
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetIncomingSelfHealingAttribute())
	{
		const float LocalIncomingSelfHealing = GetIncomingSelfHealing();
		SetIncomingSelfHealing(0.f);
		if(LocalIncomingSelfHealing > 0.f)
		{
			float Remainder = LocalIncomingSelfHealing + GetHealth() * GetIncomingDamageMultiplier();
			SetHealth(FMath::Clamp(Remainder, 0.f, GetMaxHealth()));
			float Overheal = Remainder - GetMaxHealth();
			if(Overheal > 0)
			{
				float NewArmor = GetArmor() + Overheal;
				SetArmor(FMath::Clamp(NewArmor, 0.f, GetMaxArmor()));
			} 
			
			CalculateAndSetTotalHealth(Props.TargetASC);
		}
	}

	if (Data.EvaluatedData.Attribute == GetIncomingHealingAttribute())
	{
		const float LocalIncomingHealing = GetIncomingHealing();
		SetIncomingHealing(0.f);
		if(LocalIncomingHealing > 0.f)
		{
			const float StartingHealth = GetHealth();
			const float NewHealth = StartingHealth + LocalIncomingHealing * GetIncomingHealingMultiplier() * Props.SourceASC->GetNumericAttribute(GetOutgoingHealingMultiplierAttribute());
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			UMAbilitySystemComponent* MASC = Cast<UMAbilitySystemComponent>(Props.SourceASC);

			FGameplayTagContainer ActorTags;
			FGameplayTagContainer SpecTags;
			Props.EffectContextHandle.GetOwnedGameplayTags(ActorTags, SpecTags);

			MASC->DealtHealingWithAbility.Broadcast(ActorTags);
			MASC->DealtHealingWithAbilityNative.Broadcast(ActorTags);
			const AMCharacter* SourceCharacter = Cast<AMCharacter>(Props.SourceCharacter);

			const float RealHealingDealt = GetHealth() - StartingHealth;
			


			CalculateAndSetTotalHealth(Props.TargetASC);
		}	
	}
}

void UMAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if(Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}

		if(Props.SourceController)
		{
			Props.SourceCharacter = Cast<APawn>(Props.SourceController->GetPawn());
		}
	}

	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UMAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Overhealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, LifestealPercentage, COND_None, REPNOTIFY_Always);

	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, TotalHealth, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxTotalHealth, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, StaminaRegen, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MoveSpeedMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, ReloadSpeed, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, CooldownMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, IncomingHealingMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, OutgoingHealingMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, IncomingDamageMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, OutgoingDamageMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, IncomingCriticalDamageMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, OutgoingCriticalDamageMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, IncomingKnockbackMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, OutgoingKnockbackMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Ability1Charges, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Ability2Charges, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Ability3Charges, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, PrimaryAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxPrimaryAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, SecondaryAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxSecondaryAmmo, COND_None, REPNOTIFY_Always);

	// Charge
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Charge, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxCharge, COND_None, REPNOTIFY_Always);
	
}

void UMAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Health, OldHealth);
}

void UMAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxHealth, OldMaxHealth);
}

void UMAttributeSet::OnRep_Charge(const FGameplayAttributeData& OldCharge) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Charge, OldCharge);
}

void UMAttributeSet::OnRep_MaxCharge(const FGameplayAttributeData& OldMaxCharge) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxCharge, OldMaxCharge);
}

void UMAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Armor, OldArmor);
}

void UMAttributeSet::OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxArmor, OldMaxArmor);
}

void UMAttributeSet::OnRep_Overhealth(const FGameplayAttributeData& OldOverhealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Overhealth, OldOverhealth);
}

void UMAttributeSet::OnRep_TotalHealth(const FGameplayAttributeData& OldTotalHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, TotalHealth, OldTotalHealth);
}

void UMAttributeSet::OnRep_MaxTotalHealth(const FGameplayAttributeData& OldMaxTotalHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxTotalHealth, OldMaxTotalHealth);
}

void UMAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Stamina, OldStamina);
}

void UMAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxStamina, OldMaxStamina);
}

void UMAttributeSet::OnRep_LifestealPercentage(const FGameplayAttributeData& OldLifestealPercentage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, LifestealPercentage, OldLifestealPercentage);
}

void UMAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, HealthRegen, OldHealthRegen);
}

void UMAttributeSet::OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, StaminaRegen, OldStaminaRegen);
}

void UMAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UMAttributeSet::OnRep_MoveSpeedMultiplier(const FGameplayAttributeData& OldMoveSpeedMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MoveSpeedMultiplier, OldMoveSpeedMultiplier);
}

void UMAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UMAttributeSet::OnRep_ReloadSpeed(const FGameplayAttributeData& OldReloadSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, ReloadSpeed, OldReloadSpeed);
}

void UMAttributeSet::OnRep_CooldownMultiplier(const FGameplayAttributeData& OldCooldownMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, CooldownMultiplier, OldCooldownMultiplier);
}

void UMAttributeSet::OnRep_IncomingHealingMultiplier(const FGameplayAttributeData& OldIncomingHealingMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, IncomingHealingMultiplier, OldIncomingHealingMultiplier);
}

void UMAttributeSet::OnRep_OutgoingHealingMultiplier(const FGameplayAttributeData& OldOutgoingHealingMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, OutgoingHealingMultiplier, OldOutgoingHealingMultiplier);
}

void UMAttributeSet::OnRep_IncomingDamageMultiplier(const FGameplayAttributeData& OldIncomingDamageMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, IncomingDamageMultiplier, OldIncomingDamageMultiplier);
}

void UMAttributeSet::OnRep_OutgoingDamageMultiplier(const FGameplayAttributeData& OldOutgoingDamageMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, OutgoingDamageMultiplier, OldOutgoingDamageMultiplier);
}

void UMAttributeSet::OnRep_IncomingCriticalDamageMultiplier(const FGameplayAttributeData& OldIncomingCriticalDamageMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, IncomingCriticalDamageMultiplier, OldIncomingCriticalDamageMultiplier);
}

void UMAttributeSet::OnRep_OutgoingCriticalDamageMultiplier(const FGameplayAttributeData& OldOutgoingCriticalDamageMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, OutgoingCriticalDamageMultiplier, OldOutgoingCriticalDamageMultiplier);
}

void UMAttributeSet::OnRep_IncomingKnockbackMultiplier(const FGameplayAttributeData& OldIncomingKnockbackMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, IncomingKnockbackMultiplier, OldIncomingKnockbackMultiplier);
}

void UMAttributeSet::OnRep_OutgoingKnockbackMultiplier(const FGameplayAttributeData& OldOutgoingKnockbackMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, OutgoingKnockbackMultiplier, OldOutgoingKnockbackMultiplier);
}

void UMAttributeSet::OnRep_Ability1Charges(const FGameplayAttributeData& OldAbility1Charges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Ability1Charges, OldAbility1Charges);
}

void UMAttributeSet::OnRep_Ability2Charges(const FGameplayAttributeData& OldAbility2Charges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Ability2Charges, OldAbility2Charges);
}

void UMAttributeSet::OnRep_Ability3Charges(const FGameplayAttributeData& OldAbility3Charges) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Ability3Charges, OldAbility3Charges);
}

void UMAttributeSet::OnRep_PrimaryAmmo(const FGameplayAttributeData& OldPrimaryAmmo) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, PrimaryAmmo, OldPrimaryAmmo);
}

void UMAttributeSet::OnRep_MaxPrimaryAmmo(const FGameplayAttributeData& OldMaxPrimaryAmmo) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxPrimaryAmmo, OldMaxPrimaryAmmo);
}

void UMAttributeSet::OnRep_SecondaryAmmo(const FGameplayAttributeData& OldSecondaryAmmo) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, SecondaryAmmo, OldSecondaryAmmo);
}

void UMAttributeSet::OnRep_MaxSecondaryAmmo(const FGameplayAttributeData& OldMaxSecondaryAmmo) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxSecondaryAmmo, OldMaxSecondaryAmmo);
}