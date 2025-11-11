#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
// Copyright By UmiKami



#include "AbilitySystem/AuraAbilitySystemComponent.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	BindCallbacksToDependencies();
}

void UAuraAbilitySystemComponent::BindCallbacksToDependencies()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartUpAbilities)
	{
		FGameplayAbilitySpec GameplayAbilitySpec(AbilityClass, 1);

		if (const UAuraGameplayAbility* AuraGA = Cast<UAuraGameplayAbility>(GameplayAbilitySpec.Ability))
		{
			GameplayAbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraGA->StartUpInputTag);

			GiveAbility(GameplayAbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	TArray<FGameplayAbilitySpec> GameplayAbilitySpecs = GetActivatableAbilities();

	for (auto& AbilitySpec : GameplayAbilitySpecs)
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	} 
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	TArray<FGameplayAbilitySpec> GameplayAbilitySpecs = GetActivatableAbilities();

	for (auto& AbilitySpec : GameplayAbilitySpecs)
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec,
                                                FActiveGameplayEffectHandle ActiveGEffectHandle)
{
	FGameplayTagContainer TagContainer;

	EffectSpec.GetAllAssetTags(TagContainer);

	OnEffectAssetTagsApplied.Broadcast(TagContainer);
}
