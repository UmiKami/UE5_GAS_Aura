// Copyright By UmiKami

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartUpInputTag;
	
	/**
	 * This is an optional member variable, only meant for abilities that inflict some kind of damage.
	 * It allows us to scale damage on a curve table based on ability level.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	FScalableFloat DamageMagnitude;
};
