// Copyright By UmiKami


#include "AbilitySystem/Data/AttributeInfo.h"

#include "AuraGameplayTags.h"
#include "UObject/ObjectSaveContext.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	if (const FAuraAttributeInfo* Info = AttributeInformation.Find(AttributeTag))
	{
		return *Info;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]"),
		       *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}


// region Editor Utility to Autopopulate info on data asset

void UAttributeInfo::SyncKeysToStructs()
{
	for (TPair<FGameplayTag, FAuraAttributeInfo>& Pair : AttributeInformation)
	{
		Pair.Value.AttributeTag = Pair.Key;

		if (const TSharedPtr<FGameplayTagNode> GameplayTagNode = UGameplayTagsManager::Get().FindTagNode(
			Pair.Value.AttributeTag))
		{
			Pair.Value.AttributeName = FText::FromName(GameplayTagNode->GetSimpleTagName());
			Pair.Value.AttributeDescription = FText::FromString(GameplayTagNode->GetDevComment());
		}
	}
}

#if WITH_EDITOR
void UAttributeInfo::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.PropertyChain.GetActiveNode() &&
		PropertyChangedEvent.PropertyChain.GetActiveNode()->GetValue() &&
		PropertyChangedEvent.PropertyChain.GetActiveNode()->GetValue()->GetFName() ==
		GET_MEMBER_NAME_CHECKED(UAttributeInfo, AttributeInformation))
	{
		SyncKeysToStructs();
	}
}

void UAttributeInfo::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() ==
		GET_MEMBER_NAME_CHECKED(UAttributeInfo, AttributeInformation))
	{
		SyncKeysToStructs();
	}
}
#endif

void UAttributeInfo::PostLoad()
{
	Super::PostLoad();
	SyncKeysToStructs();
}

void UAttributeInfo::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	SyncKeysToStructs();
}

// endregion
