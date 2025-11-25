// Copyright By UmiKami


#include "AbilitySystem/Abilities/AuraHitReact.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraHitReact::ApplyHitReactGameplayEffect() const
{
	checkf(HitReactEffectClass, TEXT("Please set the gameplay effect for HitReact on AuraHitReact Ability."))

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
		GetAbilitySystemComponentFromActorInfo()))
	{
		FGameplayEffectContextHandle EffectContext = AuraASC->MakeEffectContext();
		EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());

		const FGameplayEffectSpecHandle SpecHandle = AuraASC->MakeOutgoingSpec(HitReactEffectClass, 1.f, EffectContext);

		AuraASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}
}

void UAuraHitReact::RemoveHitReactGameplayEffect() const
{
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(
	GetAbilitySystemComponentFromActorInfo()))
	{
		AuraASC->RemoveActiveGameplayEffectBySourceEffect(HitReactEffectClass, AuraASC);
	}
}
