// copyright lhoffl


#include "AbilitySystem/Abilities/MCardAbility.h"

#include "MGameplayTags.h"

UMCardAbility::UMCardAbility()
{
	ActiveTag = FMGameplayTags::Get().Abilities_ActiveCard_Active;
	BlockedTag = FMGameplayTags::Get().Abilities_ActiveCard_Blocked;
	ActivationBlockedTags.AddTag(BlockedTag);
	ActivationBlockedTags.AddTag(ActiveTag);
}
