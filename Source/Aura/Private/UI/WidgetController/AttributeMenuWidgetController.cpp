// Copyright By UmiKami


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadCastAttributeInfo(
	FAuraAttributeInfo& AttributeInfo) const
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	AttributeInfo.AttributeValue = AttributeInfo.GameplayAttribute.GetNumericValue(AS);
	AttributeInfoSignature.Broadcast(AttributeInfo);
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	checkf(AttributeInfoDA, TEXT("Attribute info data asset has not been set on Attribute Menu Widget Controller"));

	for (auto& Pair : AttributeInfoDA->AttributeInformation)
	{
		BroadCastAttributeInfo(Pair.Value);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	checkf(AttributeInfoDA, TEXT("Attribute info data asset has not been set on Attribute Menu Widget Controller"));

	for (auto& Pair : AttributeInfoDA->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value.GameplayAttribute).AddLambda(
			// watchout for this, may cause a crash in the future for whatever reason due to &Pair, working fine so far 👍
			[this, &Pair](const FOnAttributeChangeData& Data)
			{
				BroadCastAttributeInfo(Pair.Value);
			}
		);
	}
}
