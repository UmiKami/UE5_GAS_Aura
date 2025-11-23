// Copyright By UmiKami

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * AureGameplayTags
 *
 * Singleton Containing Native Gameplay Tags
 */

namespace AuraGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vital_Health);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vital_Mana);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Montage_FireBolt);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effects_HitReact);
}
