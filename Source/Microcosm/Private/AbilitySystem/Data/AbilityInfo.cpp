// copyright lhoffl


#include "AbilitySystem/Data/AbilityInfo.h"

FMAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for(const FMAbilityInfo& Info : AbilityInformation)
	{
		if(Info.AbilityTag == AbilityTag) return Info;
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FMAbilityInfo();
}