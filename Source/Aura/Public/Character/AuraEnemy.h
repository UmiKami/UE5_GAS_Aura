// Copyright By UmiKami

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

enum class ECharacterClass : uint8;
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthAttributeChangeSignature, float, NewAttributeValue);

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

#pragma region Enemy Interfcace
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
#pragma endregion

#pragma region Combat Interface
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
#pragma endregion
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FHealthAttributeChangeSignature HealthSignature;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FHealthAttributeChangeSignature MaxHealthSignature;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;

protected:
	virtual void BeginPlay() override;

	virtual void SetupAbilitySystemAndAttributeSet() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* HealthBarComponent;

private:
	void BroadcastInitialValuesToUI() const;
	void OnHitReactTagChanged(FGameplayTag GameplayTag, int I);
	void BindCallbacksToDependencies();
};
