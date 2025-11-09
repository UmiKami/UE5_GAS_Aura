// Copyright By UmiKami


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(
	const FGameplayTag& InputTag, bool bLogNotFound) const
{
	const UInputAction* FoundInputAction = *AbilityInputActions.Find(InputTag);

	if (!FoundInputAction && bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find input action for given tag: %s"), *InputTag.GetTagName().ToString());
	}

	return FoundInputAction;
}
