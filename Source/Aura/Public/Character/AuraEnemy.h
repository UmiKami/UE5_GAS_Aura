// Copyright By UmiKami

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

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
#pragma endregion
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FHealthAttributeChangeSignature HealthSignature;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FHealthAttributeChangeSignature MaxHealthSignature;

protected:
	virtual void BeginPlay() override;

	virtual void SetupAbilitySystemAndAttributeSet() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* HealthBarComponent;

private:
	void BroadcastInitialValues() const;
	void BindCallbacksToChangesOnHealth();
};
