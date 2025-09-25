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

void AAuraEffectActor::DisplayAllActiveEffectAndStackCount(UAbilitySystemComponent* TargetASC)
{
	TArray<FGameplayEffectSpec> AllActiveSpecs = {};

	TargetASC->GetAllActiveGameplayEffectSpecs(AllActiveSpecs);

	for (auto Spec : AllActiveSpecs)
	{
		UE_LOG(LogTemp, Display, TEXT("Effect: %s | Count: %d "), *Spec.Def.GetName(), Spec.GetStackCount());
	}
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

		if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap && ActiveEffectHandle
			.WasSuccessfullyApplied())
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
		for (const TSubclassOf<UGameplayEffect> InstantGEClass : InstantGameplayEffectClassList)
		{
			ApplyEffectToTarget(TargetActor, InstantGEClass);
		}
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const TSubclassOf<UGameplayEffect> DurationGEClass : DurationGameplayEffectClassList)
		{
			ApplyEffectToTarget(TargetActor, DurationGEClass);
		}
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		for (const TSubclassOf<UGameplayEffect> InfiniteGEClass : InfiniteGameplayEffectClassList)
		{
			UE_LOG(LogTemp, Warning, TEXT("Added infinite effect"));
			ApplyEffectToTarget(TargetActor, InfiniteGEClass);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const TSubclassOf<UGameplayEffect> InstantGEClass : InstantGameplayEffectClassList)
		{
			ApplyEffectToTarget(TargetActor, InstantGEClass);
		}
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const TSubclassOf<UGameplayEffect> DurationGEClass : DurationGameplayEffectClassList)
		{
			ApplyEffectToTarget(TargetActor, DurationGEClass);
		}
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		for (const TSubclassOf<UGameplayEffect> InfiniteGEClass : InfiniteGameplayEffectClassList)
		{
			ApplyEffectToTarget(TargetActor, InfiniteGEClass);
		}
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
			if (const FActiveGameplayEffect* ActiveGEffect = TargetASC->GetActiveGameplayEffect(ActiveEffect))
			{
				if (const UGameplayEffect* GameplayEffect = ActiveGEffect->Spec.Def)
				{
					FString EffectNameString = GameplayEffect->GetName();
					FString DebugMessage = "Removed => ";

					UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *DebugMessage, *EffectNameString);
				}
				
				TargetASC->RemoveActiveGameplayEffect(ActiveEffect, 1);
			}
		}

		ActiveInfiniteEffectHandles.FindAndRemoveChecked(UID);
	}
}
