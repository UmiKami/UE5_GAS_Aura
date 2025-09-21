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

		TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data);
	}
}
