// Copyright By UmiKami

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraHitReact.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraHitReact : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> HitReactEffectClass;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void ApplyHitReactGameplayEffect() const;
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void RemoveHitReactGameplayEffect() const;
};
