// Copyright By UmiKami


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	checkf(GameplayEffectClass, TEXT("Aura Effect Actor Does Not Have Any Effects to Apply, please add an effect."));

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();

		EffectContextHandle.AddSourceObject(this);

		const FGameplayEffectSpecHandle EffectSpec = TargetASC->MakeOutgoingSpec(
			GameplayEffectClass, 1.f, EffectContextHandle);

		FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data);

		const bool bIsInfinite = EffectSpec.Data->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

		if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			const uint32 TargetUID = TargetActor->GetUniqueID();

			ActiveInfiniteEffectHandles.FindOrAdd(TargetUID).Add(ActiveEffectHandle);
		}
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		if (!IsValid(TargetASC))
		{
			return;
		}

		const auto UID = TargetActor->GetUniqueID();

		if (!ActiveInfiniteEffectHandles.Contains(UID))
		{
			return;
		}

		const TArray<FActiveGameplayEffectHandle> ActiveEffectList = ActiveInfiniteEffectHandles[UID];

		for (const auto& ActiveEffect : ActiveEffectList)
		{
			if (TargetASC->RemoveActiveGameplayEffect(ActiveEffect, 1))
			{
				ActiveInfiniteEffectHandles.FindAndRemoveChecked(UID);
			}
		}
	}
}
