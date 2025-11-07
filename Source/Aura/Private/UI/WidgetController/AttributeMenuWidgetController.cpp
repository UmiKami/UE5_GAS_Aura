// Copyright By UmiKami


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	checkf(AttributeInfoDA, TEXT("Attribute info has not been set on Attribute Menu Widget Controller"));

	for (auto& Pair : AttributeInfoDA->AttributeInformation)
	{
		FAuraAttributeInfo NewAttributeInfo = AttributeInfoDA->FindAttributeInfoForTag(Pair.Key);
		
		NewAttributeInfo.AttributeValue = NewAttributeInfo.GameplayAttribute.GetNumericValue(AS);
	
		AttributeInfoSignature.Broadcast(NewAttributeInfo);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}
