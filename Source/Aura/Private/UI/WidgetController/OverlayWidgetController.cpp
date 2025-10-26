// Copyright By UmiKami


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChange.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChange.Broadcast(AuraAttributeSet->GetMaxHealth());

	OnManaChange.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChange.Broadcast(AuraAttributeSet->GetMaxMana());
}

/**
* A function with multiple binds with the GAS component which,
* executes callbacks every time an attribute changes. The callbacks,
* will in turn broadcast to their respective delegates - which the
* widgets are listening on from the blueprint side.
*/
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// When the requested attribute value changes, it will call the callback function provided through AddUObject
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)
		                      {
			                      OnHealthChange.Broadcast(Data.NewValue);
		                      }
	                      );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)
		                      {
			                      OnMaxHealthChange.Broadcast(Data.NewValue);
		                      }
	                      );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)
		                      {
			                      OnManaChange.Broadcast(Data.NewValue);
		                      }
	                      );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)
		                      {
			                      OnMaxManaChange.Broadcast(Data.NewValue);
		                      }
	                      );

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->OnEffectAssetTagsApplied.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (FGameplayTag Tag : AssetTags)
			{
				const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag("Message");

				// expanding out parent tags "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False If TagToCheck is not Valid it will always return False
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}