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
	
	// Primary
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_HealthRegen, GetHealthRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MoveSpeed, GetMoveSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MoveSpeedMultiplier, GetMoveSpeedMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_IncomingKnockbackMultiplier, GetIncomingKnockbackMultiplierAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_OutgoingKnockbackMultiplier, GetOutgoingKnockbackMultiplierAttribute);
}

void UMAttributeSet::UpdateFullHealthTag(const UAbilitySystemComponent* ASC) const
{
	if (!ASC || !ASC->IsOwnerActorAuthoritative()) return;
	
	const FGameplayTag& FullHealthTag = FMGameplayTags::Get().State_FullHealth;
	if(GetHealth() == GetMaxHealth())
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
	
	if(Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}
	
	if(Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Max(GetMaxHealth(), 0.f));
		CalculateAndSetTotalHealth(Props.TargetASC);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if(LocalIncomingDamage > 0.f)
		{
			float Remainder = LocalIncomingDamage;
			
			const float NewHealth = GetHealth() - Remainder;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			CalculateAndSetTotalHealth(Props.TargetASC);

			const UMAbilitySystemComponent* MASC = Cast<UMAbilitySystemComponent>(Props.SourceASC);
			FGameplayTagContainer ActorTags;
			FGameplayTagContainer SpecTags;
			Props.EffectContextHandle.GetOwnedGameplayTags(ActorTags, SpecTags);
			MASC->DealtDamageWithAbility.Broadcast(ActorTags);
			MASC->DealtDamageWithAbilityNative.Broadcast(ActorTags);
			
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
			float Remainder = LocalIncomingSelfHealing + GetHealth();
			SetHealth(FMath::Clamp(Remainder, 0.f, GetMaxHealth()));
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
			const float NewHealth = StartingHealth + LocalIncomingHealing;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			UMAbilitySystemComponent* MASC = Cast<UMAbilitySystemComponent>(Props.SourceASC);

			FGameplayTagContainer ActorTags;
			FGameplayTagContainer SpecTags;
			Props.EffectContextHandle.GetOwnedGameplayTags(ActorTags, SpecTags);

			MASC->DealtHealingWithAbility.Broadcast(ActorTags);
			MASC->DealtHealingWithAbilityNative.Broadcast(ActorTags);

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

	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MoveSpeedMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, IncomingKnockbackMultiplier, COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, OutgoingKnockbackMultiplier, COND_None, REPNOTIFY_Always);
}

void UMAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Health, OldHealth);
}

void UMAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxHealth, OldMaxHealth);
}

void UMAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, HealthRegen, OldHealthRegen);
}

void UMAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UMAttributeSet::OnRep_MoveSpeedMultiplier(const FGameplayAttributeData& OldMoveSpeedMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MoveSpeedMultiplier, OldMoveSpeedMultiplier);
}

void UMAttributeSet::OnRep_IncomingKnockbackMultiplier(const FGameplayAttributeData& OldIncomingKnockbackMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, IncomingKnockbackMultiplier, OldIncomingKnockbackMultiplier);
}

void UMAttributeSet::OnRep_OutgoingKnockbackMultiplier(const FGameplayAttributeData& OldOutgoingKnockbackMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, OutgoingKnockbackMultiplier, OldOutgoingKnockbackMultiplier);
}