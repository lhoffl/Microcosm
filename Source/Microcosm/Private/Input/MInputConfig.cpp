// copyright lhoffl


#include "Input/MInputConfig.h"

const UInputAction* UMInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for(const FMInputAction& InputAction : AbilityInputActions)
	{
		if(InputAction.InputAction && InputAction.InputTag.MatchesTagExact(InputTag))
		{
			return InputAction.InputAction;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s]"), *InputTag.ToString());	
	}
	
	return nullptr;	
}
